#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "builtins.hpp"

#define i64_t long long
typedef unsigned long size_t;

extern "C"
{
    std::vector<void*> last_heaps;
    std::unordered_map<i64_t, std::vector<void*>> heaps;
    std::vector<i64_t> last_ids;
    std::unordered_map<i64_t, std::vector<i64_t>> dependencies;
    std::unordered_map<i64_t, std::vector<i64_t>> calling_ids_to_free;
    i64_t unique_number;
}

extern "C" i64_t get_unique_number()
{
    unique_number += 1;
    return unique_number;
}

extern "C" i64_t* use_heap(i64_t size)
{
    auto heap_address = malloc(size);
    last_heaps.push_back(heap_address);
    return (i64_t*)heap_address;
}

extern "C" void register_calling_id()
{
    auto id = get_unique_number();
    // printf("%lu",last_heaps.size());
    heaps.insert(std::make_pair(id, last_heaps));
    last_heaps = {};
    dependencies.insert(std::make_pair(id, last_ids));
    last_ids = {};
}

extern "C" void register_calling_id_to_free(i64_t session_id)
{
    auto id = get_unique_number();
    // printf("%lu\n!",last_heaps.size());
    heaps.insert(std::make_pair(id, last_heaps));
    last_heaps = {};
    dependencies.insert(std::make_pair(id, last_ids));
    last_ids = {};
    calling_ids_to_free[session_id].push_back(id);
}

extern "C" void free_heaps_associated_calling_id(i64_t calling_id)
{
    auto associated_calling_ids_to_free = calling_ids_to_free[calling_id];
    for (auto& id : associated_calling_ids_to_free)
    {
        free_heaps(id);
    }
}

extern "C" void free_heaps(i64_t id)
{
    if (dependencies.count(id))
    {
        auto dependensies = dependencies[id];
        for (auto& dependency : dependensies)
        {
            free_heaps(dependency);
        }
        for (auto& heap : heaps[id])
        {
            free(heap);
        }
    }
}

extern "C" void* malloc(size_t __size);
extern "C" void* memcpy(void* __dst, const void* __src, size_t __n);
extern "C" void exit(int);
extern "C" int printf(const char*, ...);
extern "C" int snprintf(char* __str, size_t __size, const char* __format, ...);
extern "C" void* realloc(void* __ptr, size_t __size);
extern "C" int puts(const char*);
extern "C" char* strcat(char* __s1, const char* __s2);
extern "C" int strcmp(const char* __s1, const char* __s2);

// including headers makes cridge file too big, so forward declarations are
// written here.

extern "C" void* clawn_realloc(void* array, i64_t element_size,
                               i64_t allocated_size)
{
    void* ptr = realloc(array, element_size * allocated_size);
    if (ptr == nullptr)
    {
        puts(
            "\x1b[31mCRITICAL ERROR:\x1b[39m failed to realloc at appending "
            "element to list");
        exit(1);
    }
    return ptr;
}

extern "C" void clawn_memcpy(void* array, i64_t size, i64_t element_size,
                             void* element)
{
    memcpy((char*)array + (size * element_size), &element, element_size);
}

extern "C" void* clawn_index(void* array, i64_t size, i64_t element_size,
                             i64_t index)
{
    if (0 <= index && index < size)
    {
        return (char*)array + element_size * index;
    }
    else
    {
        puts("Error: list index out of range");
        exit(1);
    }
}

extern "C" void clawn_set_element(void* array, i64_t size, i64_t element_size,
                                  void* element, i64_t index)
{
    if (0 <= index && index < size)
    {
        *((char**)((char*)array + element_size * index)) = (char*)element;
    }
    else
    {
        puts("Error: list index out of range");
        exit(1);
    }
}

extern "C" _List* list_constructor(i64_t element_size)
{
    _List* self = (_List*)malloc(sizeof(_List));
    self->pointer = nullptr;
    self->element_size = element_size;
    self->size = 0;
    self->allocated_size = 1;
    return self;
}

extern "C" void append(_List* list, void* new_element)
{
    if (list->size + 1 >= list->allocated_size)
    {
        list->allocated_size = list->allocated_size * 2;
        list->pointer = clawn_realloc(list->pointer, list->element_size,
                                      list->allocated_size);
    }
    clawn_memcpy(list->pointer, list->size, list->element_size, new_element);
    list->size += 1;
}

extern "C" void* get(_List* list, i64_t index)
{
    return clawn_index(list->pointer, list->size, list->element_size, index);
}

extern "C" void print(String* string) { printf("%s\n", string->string); }

extern "C" String* string_constructor(const char* str, i64_t size)
{
    String* string = (String*)malloc(sizeof(String));
    string->string = const_cast<char*>(str);
    string->size = size;
    return string;
}

extern "C" String* get_appended_string(String* string1, String* string2)
{
    char* new_str;
    new_str = (char*)malloc(string1->size + string2->size);
    new_str[0] = '\0';
    strcat(new_str, string1->string);
    strcat(new_str, string2->string);
    return string_constructor(new_str, string1->size + string2->size);
}

extern "C" String* input()
{
    std::string input;
    std::cin >> input;
    char* char_ptr;
    char_ptr = (char*)malloc(input.size());
    char_ptr[0] = '\0';
    strcat(char_ptr, input.c_str());
    return string_constructor(char_ptr,input.size());
}

extern "C" char* to_char_ptr(String* string) { return string->string; }

extern "C" void append_string(String* string, String* to_add)
{
    char* new_str;
    new_str = (char*)malloc(string->size + to_add->size);
    new_str[0] = '\0';
    strcat(new_str, string->string);
    strcat(new_str, to_add->string);
    string->size += to_add->size;
    string->string = new_str;
}

extern "C" String* int_to_str(i64_t n)
{
    i64_t n_copy = n;
    i64_t size;
    if (n >= 0)
    {
        size = 1;
        while (n)
        {
            n /= 10;
            size++;
        }
    }
    else
    {
        size = 2;
        n = -n;
        while (n)
        {
            n /= 10;
            size++;
        }
    }
    char* str = (char*)malloc(size);
    snprintf(str, size + 1, "%lld", n_copy);
    return string_constructor(str, size);
}

extern "C" String* float_to_str(double n)
{
    int digit = 16;
    char* str = (char*)malloc(digit);
    snprintf(str, digit + 1, "%lf", n);
    return string_constructor(str, digit);
}

extern "C" double to_real_number(i64_t x) { return x; }

extern "C" i64_t to_integer(double x) { return x; }

extern "C" bool op_and(bool x, bool y) { return x && y; }

extern "C" bool op_or(bool x, bool y) { return x || y; }

extern "C" bool op_equal_string(String* str1, String* str2)
{
    return strcmp(str1->string, str2->string) == 0;
}

extern "C" i64_t add_int(i64_t x, i64_t y) { return x + y; }

extern "C" i64_t sub_int(i64_t x, i64_t y) { return x - y; }

extern "C" i64_t mul_int(i64_t x, i64_t y) { return x * y; }

extern "C" bool op_greater_than_int(i64_t x, i64_t y) { return x > y; }

extern "C" bool op_greater_than_or_equal_int(i64_t x, i64_t y)
{
    return x >= y;
}

extern "C" bool op_less_than_int(i64_t x, i64_t y) { return x < y; }

extern "C" bool op_less_than_or_equal_int(i64_t x, i64_t y) { return x <= y; }

extern "C" bool op_equal_int(i64_t x, i64_t y) { return x == y; }

extern "C" bool op_not_equal_int(i64_t x, i64_t y) { return x != y; }

extern "C" double add_double(double x, double y) { return x + y; }

extern "C" double sub_double(double x, double y) { return x - y; }

extern "C" double mul_double(double x, double y) { return x * y; }

extern "C" double div_double_double(double x, double y) { return x / y; }

extern "C" double div_double_int(double x, i64_t y) { return x / y; }

extern "C" double div_int_double(i64_t x, double y) { return x / y; }

extern "C" double div_int_int(i64_t x, i64_t y) { return x / y; }

extern "C" bool op_greater_than_double(double x, double y) { return x > y; }

extern "C" bool op_greater_than_or_equal_double(double x, double y)
{
    return x >= y;
}

extern "C" bool op_less_than_double(double x, double y) { return x < y; }

extern "C" bool op_less_than_or_equal_double(double x, double y)
{
    return x <= y;
}

extern "C" bool op_equal_double(double x, double y) { return x == y; }

extern "C" bool op_not_equal_double(double x, double y) { return x != y; }