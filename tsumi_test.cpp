// tsumi_selftest.cpp  ― tsumi() 単体テスト（座標入力）
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>

/*  ─────────────────────────────── *
 *  #include 前に  main  を別名へ書き換え             *
 *  → 取り込み終了後に  #undef で元に戻す             */
#define main YONMOKU_ORIGINAL_MAIN
#include "yonmoku_test_baaiwake_ps.cpp"     // tsumi / Board を取り込む
#undef main
/* ──────────────────────────────────────────────── */

/*---------- 座標列 → Board ----------*/
Board board_from_moves(const std::vector<std::tuple<int,int,int>>& mv){
    Board b;
    for (size_t t=0;t<mv.size();++t){
        auto [x,y,z]=mv[t];
        int bit=x+y*SIZE+z*SIZE*SIZE;
        (t%2?b.You:b.Me)|=1ULL<<bit;
    }
    if(mv.size()%2) std::swap(b.Me,b.You);  // 奇数手なら手番入替
    return b;
}
/*-----------------------------------*/

/* ---------- 盤面を人が読める形で表示 ---------- *
 * z=3 (最上段) から z=0 (最下段) へ順に 4×4 グリッドを出力         */
void print_board(const Board& b)
{
    std::cout << "    z=1  z=2  z=3  z=4\n";
    for (int y = SIZE - 1; y >= 0; --y) {
        std::cout << "y=" << y + 1 << ' ';
        for (int z = 0; z < SIZE; ++z) {
            for (int x = 0; x < SIZE; ++x) {
                int bit = x + y * SIZE + z * SIZE * SIZE;
                char c  = '-';
                if (b.Me  >> bit & 1) c = 'X';   // 先手石
                if (b.You >> bit & 1) c = 'O';   // 後手石
                std::cout << c;
            }
            std::cout << (z == SIZE - 1 ? '\n' : ' ');
        }
    }
    std::cout << "  x=";
    for (int z = 0; z < SIZE; ++z) {
        std::cout << "1234" << (z == SIZE - 1 ? '\n' : ' ');
    }
}

/*  テストケース  { 座標列, 期待 tsumi }  */
const std::vector<std::pair<
      std::vector<std::tuple<int,int,int>>, int>> CASES={
    //
    {{{0,0,0},{1,1,0},{0,3,0},{1,3,0},{1,0,0},{2,0,0},{1,2,0},{2,2,0},{2,1,0},{2,3,0},
		{3,1,0},{3,0,0},{3,3,0},{3,2,0},{3,3,1},{1,1,1},{1,0,1},{2,1,1},{1,2,1},{3,2,1},{2,2,1},{2,1,2},{3,1,1},
		{2,2,2},{1,1,2},{2,1,3},{3,2,2},{2,2,3}}, +1},
    // 
    {{{0,0,0},{3,0,0},{0,1,0},{0,0,1},{1,0,0},{2,1,0},{1,1,0},{1,1,1},{3,2,0},{3,2,1}}, +1},
    // 
    {{{0,0,0},{0,1,0},{0,2,0},{1,2,0},{0,3,0},{2,1,0},{1,1,0},{3,0,0},{2,2,0},{3,1,0},{3,2,0},{3,3,0},
		{1,1,1},{2,1,1},{1,2,1},{1,1,2},{2,2,1},{2,1,2},{2,1,3},{2,2,2}}, +1},
};

int main(){
	// 76本の勝利ラインを読み込む
    {
		vector<unsigned long long> lines;
		for (int x = 0; x < SIZE; x++) for (int y = 0; y < SIZE; y++)
		{
			unsigned long long line = 0uLL;
			for (int z = 0; z < SIZE; z++) line |= BIT(x, y, z);
			lines.push_back(line);
		}
		for (int y = 0; y < SIZE; y++) for (int z = 0; z < SIZE; z++)
		{
			unsigned long long line = 0uLL;
			for (int x = 0; x < SIZE; x++) line |= BIT(x, y, z);
			lines.push_back(line);
		}
		for (int z = 0; z < SIZE; z++) for (int x = 0; x < SIZE; x++)
		{
			unsigned long long line = 0uLL;
			for (int y = 0; y < SIZE; y++) line |= BIT(x, y, z);
			lines.push_back(line);
		}
		for (int x = 0; x < SIZE; x++)
		{
			unsigned long long line1 = 0uLL, line2 = 0uLL;
			for (int yz = 0; yz < SIZE; yz++)
			{
				line1 |= BIT(x, yz, yz);
				line2 |= BIT(x, yz, SIZE - 1 - yz);
			}
			lines.push_back(line1);
			lines.push_back(line2);
		}
		for (int y = 0; y < SIZE; y++)
		{
			unsigned long long line1 = 0uLL, line2 = 0uLL;
			for (int zx = 0; zx < SIZE; zx++)
			{
				line1 |= BIT(zx, y, zx);
				line2 |= BIT(zx, y, SIZE - 1 - zx);
			}
			lines.push_back(line1);
			lines.push_back(line2);
		}
		for (int z = 0; z < SIZE; z++)
		{
			unsigned long long line1 = 0uLL, line2 = 0uLL;
			for (int xy = 0; xy < SIZE; xy++)
			{
				line1 |= BIT(xy, xy, z);
				line2 |= BIT(xy, SIZE - 1 - xy, z);
			}
			lines.push_back(line1);
			lines.push_back(line2);
		}
		{
			unsigned long long line1 = 0uLL, line2 = 0uLL, line3 = 0uLL, line4 = 0uLL;
			for (int xyz = 0; xyz < SIZE; xyz++)
			{
				line1 |= BIT(xyz, xyz, xyz);
				line2 |= BIT(xyz, xyz, SIZE - 1 - xyz);
				line3 |= BIT(xyz, SIZE - 1 - xyz, xyz);
				line4 |= BIT(xyz, SIZE - 1 - xyz, SIZE - 1 - xyz);
			}
			lines.push_back(line1);
			lines.push_back(line2);
			lines.push_back(line3);
			lines.push_back(line4);
		}
		assert(lines.size() == LINES_NUM);
		for (int j = 0; j < LINES_NUM; j++)
		{
			LINES[j] = lines[j];
			assert(__builtin_popcountll(LINES[j]) == 4);
			assert(Board::win(LINES[j]) == State::End);
		}

		unsigned long long ret = 0uLL;
		auto dfs = [&](auto self, int id, unsigned long long B, int rest) -> void
		{
			if (id == 64)
			{
				if (Board::win(B) == State::Continue)
				{
					//ret |= Board::reach_naive(B);
					//ret |= Board::reach(B);
					//assert(Board::reach_naive(B) == Board::reach(B));
				}
			}
			else
			{
				self(self, id + 1, B, rest);
				if (rest > 0) self(self, id + 1, B | 1uLL << id, rest - 1);
			}
		};
		//validation
		//dfs(dfs, 0, 0uLL, 6);
	}                 
	// ここからテスト開始 
    int fail=0;
    for(size_t i=0;i<CASES.size();++i){
        Board bd=board_from_moves(CASES[i].first);
        //int got=proof_search(bd,54), want=CASES[i].second;
		int got  = proof_search(bd, 54).verdict;
		int want = CASES[i].second;
        if(got==want)
            std::cout<<"Case "<<i+1<<" PASS (tsumi="<<got<<")\n";
        else{
            std::cout<<"Case "<<i+1<<" FAIL want "<<want<<" got "<<got<<"\n";
            ++fail;
        }
        print_board(bd);  
    }
    std::cout<<(fail?"Some tests failed \n":"All tests passed \n");
    return fail;
}
