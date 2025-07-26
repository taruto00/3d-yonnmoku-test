550-792行にかけてproof_searchを実装, αβ法探索に831-833行で組み込み，探索で現れる各局面でply_limitだけ手を読む. 

3d-yonnmoku-v2, solve_mate.py では詰み手順も表示していたが, tsumi_test.cppでは詰み手順は表示していない. (proof_searchのmvを表示すれば良いので, そこについては直ぐに変更できる. )
tsumi_test.cpp main関数直前で局面を作れば, その局面の詰みを探索できる, またmain内のproof_search呼び出しで読み込み手数も変更可（デフォルトは27手）. 


このコードは友人のリポジトリ： https://github.com/quasimaster/yonmoku に多少のアルゴリズムを付け加えたものです. 
