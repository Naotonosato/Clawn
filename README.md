# Clawn
Clawnは人間にとっての扱いやすさを第一に設計されたプログラミング言語です。

![demo](https://user-images.githubusercontent.com/33174108/199721317-3729b791-fa9b-49a6-af59-a36d5cf9e8a0.gif)

平易な文法と高度な型システム、優れたC FFIに静的メモリ管理機構を備えており、安全で高品質なプログラムを簡単に書けるようになっています。

## 使い方

### 動作環境

Clawnは現在Mac OS上でのみ動作が確認されています。Mac OS以外のOSをご利用の方は今後Clawnをインストール済みのDocker Imageを配布する予定ですので少々お待ちください。

## インストール方法

Mac OS上でClawnを利用するには、Clang,Python3,libclang（Pythonのライブラリ）が必要になるため、これらのソフトウェアを事前にインストールしてください。

上記のソフトウェアがインストールできたらClawnのソースコードを本リポジトリからダウンロードし、展開した後、Clawn/buildディレクトリに移動し、

`./clawn 実行したいプログラムファイル名`を実行すると、プログラムが実行されます。

ご自身でソースコードからビルドしたい方は、bison3.8.2, flex2.6.4, cmake3.19.4, llvm9.0をインストールしたうえで

`cmake ..`

`make`

を実行してください。これらのコマンドの実行が完了すると、Clawn/build下にclawnという実行可能ファイルが生成され、Clawnを利用できるようになります。



## 文法

Clawnは自然言語に近い直感的な文法を採用しており、わかりやすさを最重要視して設計されています。

+ 変数定義、代入

```c++
a = 10
a = 100
```

Clawnでは入力ファイルのグローバルスコープがエントリーポイントに設定されるため、int main()等の宣言は不要で、すぐにプログラムを書き始められます。

+ 関数呼び出し

```c++
print("Hello, World!")
```

出力:

```
Hello, World!
```



+ block式

```c++
result = {
	print("Block expression")
	=>0
	print("End of Block expression")
}
print("Result is " + int_to_str(result))
```

出力:

```
Block expression
End of Block expression
Result is 0
```

中括弧の中に一つ以上の式を書く事で構成されるブロックは、途中に`=> expr`という記述があると`expr`の値をブロックの値として返します。これは処理を程よい粒度でまとめ、プログラムをわかりやすくするのに役立ちます。ブロック中に`=>`が現れなかった場合、ブロックは値を返しません。

+ if式

```c++
x = 100
message = if x == 100
{
	print("x is 100.")
	=>"OK."
} else
{
	=>"Something went wrong."
}
print(message)
```

出力:

```shell
x is 100.
OK.
```

Clawnにおけるifは文ではなく式で、block同様に値を返します。



+ repeat式

```python
result = repeat count:count<5
    {
        print("count: " +int_to_str(count))
        =>count
    }
print(int_to_str(result))
```

出力:

```
count: 0
count: 1
count: 2
count: 3
count: 4
4
```

repeat式は、repeat `カウンター用変数名`:`条件`とすることで`条件`が満たされている間カウンター用変数をインクリメントし、続く式を繰り返し実行するというもので、if,block同様に値を返します。

+ 参照

```c++
value = "value"
reference = refer value
print(access reference)
```

出力:

```
value
```

`refer`で参照を得て、`access`で参照先の値を取得できます。

+ 関数

```c++
function f(x)
{
	return x
}

f(0)
print(f("abc"))
a = f
print(float_to_str(a(0.1)))
```

出力:

```
abc
0.100000
```



関数はfunction `関数名` (`引数1`,`引数2`...)の形で多相関数として定義されます。型注釈は不要ですが渡される引数の型を制限するために後述の`要件`を引数に対して指定することもできます。また、Clawnにおける関数は第一級オブジェクトなので変数に代入したり、関数に引数として渡したりもできます。

+ 構造体

```c++
structure S
{
	x
	y
}

instance = S{x:10.0,y:0.0}
print(float_to_str(instance.x))
```

出力:

```
10.000000
```

構造体は

structure `構造体名`

{

`メンバー名`...

}

の形で定義され、`構造体名`{`メンバー名`:`expr`...}の形でインスタンス化できます。

関数同様に後述の`要件`をメンバーに対して適応できます。

+ 直和型

```c++
union U
{
	A
	B
}

u = U#A(0)
u = U#B("abc")

str = match u
{
	A=>{
        =>int_to_str(u.A)
    }
	B=>{
        =>u.B
    }
}
print(str)
```

出力:

```
abc
```

直和型は 

Union `名前`

{

`タグ`...

}

の形で定義され、`直和型名`#`タグ名`(`expr`)の形でインスタンスを作成できます。

また、match式によって保持している値によって処理を切り替えることも可能です。

+ 要件

```
requirement HasWidth
{
	has: width: RealNumber
}

function f(x:HasWidth)
{
	return x.width
}

f(0)
```

出力:

```powershell
Error: requirement 'HasWidth(has member named 'width': RealNumber)' is set at line 7 but not satisfied with type 'Integer'
```

要件は関数の引数、構造体のメンバ、直和型のタグに指定でき、

+ 型名
+ 要件句
+ 要件名

のいずれかです。要件句は

+ 値型であることを要求する `value`
+ 要件`X`を満たす値型であることを要求する`value:X`
+ 参照型であることを要求する`reference`
+ 要件`X`を満たす型への参照型であることを要求する`reference:X`
+ `name`というメンバーを持つことを要求する`has: name`
+ 要件`X`を満たすメンバー`name`を持つことを要求する`has:name:X`

によって構成されており、今後引数`arg1,arg2...`で呼び出し可能であることを要求する`callable_with`や返り値の型が要件`X`を満たすことを要求する`returns:X`などを実装する予定です。

また、要件名は　

requirement `要件名`

{

`要件`...

}

とすることで定義できます。

この機能によって意図に反するプログラムが実行されてしまうことを防げます。

## C FFI

ClawnではC言語の関数を容易に呼び出せます。

```python
import "source.c"

c_function()
```

このように、import `"C言語のプログラムのファイルの場所"`とするだけで、そのCプログラム中の関数を呼び出せます。

## メモリ管理

Clawnはプログラム中の値の依存関係を静的に検証し、適切なタイミングでメモリの確保と解放を行うコードを自動的に挿入するメモリ管理機構を持っているため、プログラマがメモリについて意識する必要はありません。

ただし、この機能は実験的で今後さらなる安定化及び最適化がなされる予定です。

## ロードマップ
+ コンパイル速度の改善
+ エラーメッセージの改善
+ メモリ管理機構の改善
+ 最適化の強化
+ iOS,Android,Windows,Linux,Web出力への対応
+ 標準ライブラリの整備
+ ビルドシステムの開発
+ パッケージマネージャーの開発