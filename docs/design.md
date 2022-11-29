# コンセプト
Clawnは安全で高速なプログラムを簡単に書けるようにすることを目標に開発されたプログラミング言語です。
静的型付けコンパイル型言語として実装されていながら、初歩的なプログラムにおいて型やメモリに関する前提知識を意識する必要がないように設計されており、静的型付けによる恩恵と学習曲線のなだらかさを両立しています。
# 特徴的な機能
Clawnに実装されている特徴的な機能としては
+ シンプルな構文
+ 特殊な型システム
+ 静的解析に基づいた自動メモリ管理（実験段階）
+ 簡便なC FFI

といったものが挙げられます。これらは一貫して上記のコンセプトを実現するために実装されています。
## 構文
全ての構文を説明すると長くなってしまうので、[README](https://github.com/Naotonosato/Clawn)をご参照ください。
構文を考える際意識したこととしては、学習の容易さのために省略語や記号をなるべく使わないようにする、という事がありますが、何か科学的根拠に基づいているというわけではないです。
## 型システム
Clawnの型システムの特徴は
+ 部分型が存在しない
+ 関数が全て多相的である
+ Higher rank polymorphismに対応している

の二点に集約されます。
関数を全て多相関数として扱うことで、型名を明示しなくて済み初学者が早い段階で型について意識しなくて済む他、部分型の使用感の一部を再現することが可能になっています。
部分型を採用している言語では、関数呼び出しや変数への代入で、暗黙の内により大きい型（≒派生クラスに対する基底クラス）への変換がなされます。Clawnではこのうち関数呼び出しについてのみ、関数が**全て**多相関数として扱われていることで、表面上同様の振る舞いをします。nominal typingを採用している言語（C+,Java等）では型A,Bが構造的にはA < BであってもAとBが継承関係にない場合暗黙のキャストは行われず、「偶然メンバー名が同じだけで実際には互換性がない」型同士が同一に扱われることを阻止していますが、Clawnでは`requirement`という、型が満たすべき要件を記述する仕組みでこれと同等の機能を提供しています。
また、Higher rank polymorphism（多相関数を引数にとる多相関数を引数に取る...多相関数 のような関数を扱えること）を実現するに際し、Clawnでは
+ 関数にnominal typingを適用する
+ 関数の引数型を拡張可能バリアントとする

ことでHM型推論に若干のアレンジを加えた程度の複雑さでの実装を可能にしています。
このアレンジとは、全ての関数呼び出しについて、どの関数がどのような引数で呼び出されたかを記録し、関数の型を

`（取り得る引数の型パターンの候補をタグに取るバリアント）→返し得る返り値の候補をタグに取るバリアント`

とすることです。

```c++
function f(x)
{
    return x
}//この時点ではf:(α)->α

f(10) //この時点ではf:{(0:Integr})->{0:Integer}
f("a")//f:({0:Integr|1:String})->{0:Integer|1:String}
```

（関数の型付けの例）

関数に関してnominal typingを採用していることで、名前の違う関数は名前以外が完全に同一でも異なる型を持つものとして扱われることから、全ての関数呼び出しについて、どこで定義された関数が呼び出されているのかという情報を得ることが可能なため、上述のような仕組みが実現されています。

そして、関数は型推論が終了し型が確定すると以下のように展開され単相化されます。

```c++
function f({0:Integr|1:String} x) -> {0:Integer|1:String}
{
	if (x.tag == 0) return f_0(x as Integer)
	if (x.tag == 1) return f_1(x as String)
}

function f_0(Integer x) ->Integer { return x}
function f_1(String x) ->String{ return x}
```

（展開された関数の疑似コード）

以上のような仕組みにより、Clawnでは多相関数を実際にアドレスを持つ第一級オブジェクトとして扱うことが可能になっています。通常のHindley Milner型推論では以下のようなプログラムは上手く扱えません。
通常のHindley Milner型推論では以下のようなプログラムは（型注釈なしでは）上手く扱えません。

```ocaml
# let f x = x(x 100);;
val f : (int -> int) -> int = <fun>
# let g x = "string";;
val g : 'a -> string = <fun>
# f g;;
Error: This expression has type int -> string
       but an expression was expected of type int -> int
       Type string is not compatible with type int 
```

（OCamlで多相関数を引数に渡す例）

しかし、Clawnでは

```c++
function f(x)
{
    return x(x(100))
}

function g(x)
{
    return "string"
}

print(f(g))
```

出力:

```
string
```

このように多相関数が引数に渡されても正しく処理できます。現時点ではこの実装は実行時のオーバーヘッドを生みますが、より具体的な関数呼び出しに置き換える最適化が実装可能なのは明らかなので、将来的に改善される予定です。
また、現時点ではClawnでは[再帰関数が定義できません](https://github.com/Naotonosato/Clawn/issues/2)が、おそらく型システムの深い部分に由来するものではなく、コード生成の実装に由来するもので、比較的少ない変更で修正できるものと考えています。

## メモリ管理
Clawnではメモリ管理は自動で行われ、プログラマがメモリ管理について意識する必要がないようになっています。
具体的には
1. 関数中の項の依存関係を解析する
2. 関数中の`return`式と依存関係にある項のうち、メモリ確保を必要とするものを列挙する
3. 2.のステップで得られた項のメモリ確保をヒープ領域を利用するものに置き換える 

ことでメモリ確保の記述を自動化しており、さらに、
4. 全ての関数の先頭にユニークな整数値(session id)を生成するコードを埋め込む
5. 全てのヒープ確保にユニークな整数値(allocation id)を割り当てる
6. 全ての関数呼び出しにユニークな整数値（calling id）を割り当てる

ことでヒープ領域の確保がどの関数呼び出しで発生したのかを記録できるようにし、
7. 1.のステップで得られた解析結果から、どんな`return`式とも依存関係を持たない関数呼び出しについて、そのcalling idに紐づいたメモリを解放するコードを埋め込む 

というステップにより自動メモリ管理を実現しています。しかし現時点ではメモリリークが必要以上に発生する他、このアルゴリズムが本当に安全なのかどうかも未証明で、実験段階にある機能です。

(これらのステップは https://github.com/Naotonosato/Clawn/blob/main/src/lib/mir/analyzer.cppやhttps://github.com/Naotonosato/Clawn/blob/main/src/lib/compiler/pipeline/mir_to_llvm_ir.cpp 、　https://github.com/Naotonosato/Clawn/blob/main/src/lib/compiler/builtins.cpp　で実装されています。)

## C FFI

Clawnでは、言語の実用性という観点から過去の資産へのアクセスを重視しており、

`import ファイル名.c`

とするだけでC言語の関数を呼び出すことができるような仕組みが実装されています。

例えば

```c
typedef struct Position {
    double x;
    double y;
} Position;

Position get_position()
{
    Position pos;
    pos.x = 0.0;
    pos.y = 0.0;
    return pos;
}
```

(pos.c)

このようなC言語のプログラムと

```
import "path/to/pos.c"

pos = get_position()
print(float_to_str(pos.x))
```

このようなClawnのプログラムがあった際、コンパイラは付属の[cridge](https://github.com/Naotonosato/Clawn/blob/main/tools/cridge.py)というツールを用いて

```python
@Ctype String
    @string : __PTR__ __C_CHAR__SIZE_1
    @size : __C_LONGLONG__SIZE_8
@Cfunction print
    arguments: __PTR__ String
    return: __C_VOID__
```

このようなC言語の関数及び構造体の情報を記したファイルを生成し、この情報をもとに自動でバインディングを行います。

（cridgeはlibclangを用いてプログラム中に現れる関数や構造体の情報を収集し、Clawnに処理できる形に変換するPythonスクリプトです）
