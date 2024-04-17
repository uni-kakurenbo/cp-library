競技プログラミング用の C++ ライブラリです。  
ツギハギ設計です。主に依存関係がやばい。修正される予定はないです。

---
- `action/`: Lazy Segment Tree などに乗る構造．
- `adaptor/`: 主に，STL のデータ構造や入出力のラッパー．
- `algebraic/`: よく使う代数構造の定義や，手軽に定義するためのヘルパー．
- `convolution/`: 畳み込み．
- `data_structure/`: データ構造．
  - `adaptor/`: Fenwick Tree を Set 風に使うためのアダプターなど．
- `debugger/`: デバッガー．
- `geometry/`: 初等幾何．
- `global/`: 主に各種の定数．
- `graph/`: グラフ理論全般．木に関するものも含まれる．
- `hash/`: 列や集合のハッシャー
- `include/`: `#include` の詰め合わせ．`all.hpp` もある．
- `iterable/`: 主に列やイテレータの組に対する操作．
- `macro/`: マクロ．主にライブラリ内部で使うもの．`internal/` のサブディレクトリにしてもいいかも．
- `numeric/`: 主に，数学に関するアルゴリズムと行列．
- `random/`: 乱数生成器とそのラッパー．
- `sneaky/`: 未定義動作を引き起こす恐れのあるマクロなど．
- `snippet/`: 便利のためのマクロや定数など．コンテスト中にも使うもの．
- `structure/`: グラフ (隣接リスト)，グリッド．
- `template/`: コンテスト中によく使うライブラリを一括で `#include` したり `using` したりするヘッダ．
- `utility/`: その他の便利系ライブラリ．
- `view/`: 列のビュー．基本的には `std::ranges::views::` を踏襲している．
- `**/internal/`: 主に内部実装．concepts やメタ関数も含まれる．コンテスト中に個別で `#include` することは想定していない．

---
[**Expanded header (standard)**](./extended/expanded.hpp)  
[**Expanded header (whole)**](./expanded/all.hpp)  

[**Lint report**](./lint)
