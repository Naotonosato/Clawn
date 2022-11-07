#include <cstdint>
#define i64_t long long
typedef unsigned long size_t;

extern "C" i64_t get_unique_number();
extern "C" i64_t* use_heap(i64_t size);
extern "C" void register_calling_id();
extern "C" void register_calling_id_to_free(i64_t session_id);
extern "C" void free_heaps_associated_calling_id(i64_t calling_id);
extern "C" void free_heaps(i64_t id);

extern "C" void* malloc(size_t __size);
extern "C" void* memcpy(void* __dst, const void* __src, size_t __n);
extern "C" void exit(int);
extern "C" int printf(const char*, ...);
extern "C" int snprintf(char* __str, size_t __size, const char* __format, ...);
extern "C" void* realloc(void* __ptr, size_t __size);
extern "C" int puts(const char*);
extern "C" char* strcat(char* __s1, const char* __s2);
extern "C" int strcmp(const char* __s1, const char* __s2);

struct String;

typedef struct String
{
    char* string;
    i64_t size;
} String;

typedef struct _List
{
    i64_t size;
    i64_t allocated_size;
    i64_t element_size;
    void* pointer;
} _List;

extern "C" void* clawn_realloc(void* array, i64_t element_size,
                               i64_t allocated_size);

extern "C" void clawn_memcpy(void* array, i64_t size, i64_t element_size,
                             void* element);

extern "C" void* clawn_index(void* array, i64_t size, i64_t element_size,
                             i64_t index);

extern "C" void clawn_set_element(void* array, i64_t size, i64_t element_size,
                                  void* element, i64_t index);

extern "C" _List* list_constructor(i64_t element_size);

extern "C" _List* get_appended_list(_List* list, void* new_element);

extern "C" void append(_List* list, void* new_element);

extern "C" void* get(_List* list, i64_t index);

extern "C" void print(String* string);

extern "C" String* string_constructor(const char* str, i64_t size);

extern "C" String* input();

extern "C" String* get_appended_string(String* string1, String* string2);

extern "C" char* to_char_ptr(String* string);

extern "C" void append_string(String* string, String* to_add);

extern "C" String* int_to_str(i64_t n);

extern "C" String* float_to_str(double n);

extern "C" double to_real_number(i64_t x);

extern "C" i64_t to_integer(double x);
extern "C" bool op_and(bool x, bool y);

extern "C" bool op_or(bool x, bool y);

extern "C" bool op_equal_string(String* str1, String* str2);

extern "C" i64_t add_int(i64_t x, i64_t y);

extern "C" i64_t sub_int(i64_t x, i64_t y);

extern "C" i64_t mul_int(i64_t x, i64_t y);

extern "C" bool op_greater_than_int(i64_t x, i64_t y);

extern "C" bool op_greater_than_or_equal_int(i64_t x, i64_t y);

extern "C" bool op_less_than_int(i64_t x, i64_t y);

extern "C" bool op_less_than_or_equal_int(i64_t x, i64_t y);

extern "C" bool op_equal_int(i64_t x, i64_t y);

extern "C" bool op_not_equal_int(i64_t x, i64_t y);

extern "C" double add_double(double x, double y);

extern "C" double sub_double(double x, double y);

extern "C" double mul_double(double x, double y);

extern "C" double div_double_double(double x, double y);

extern "C" double div_double_int(double x, i64_t y);

extern "C" double div_int_double(i64_t x, double y);

extern "C" double div_int_int(i64_t x, i64_t y);

extern "C" bool op_greater_than_double(double x, double y);

extern "C" bool op_greater_than_or_equal_double(double x, double y);

extern "C" bool op_less_than_double(double x, double y);

extern "C" bool op_less_than_or_equal_double(double x, double y);

extern "C" bool op_equal_double(double x, double y);

extern "C" bool op_not_equal_double(double x, double y);