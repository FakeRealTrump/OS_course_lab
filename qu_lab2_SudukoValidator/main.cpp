#include <cstdio>
#include"SudokoValidator.h"
int main()
{
	vector<int> a({ 9, 8, 7, 6, 5, 4, 3, 2, 1 });
	Grid test(a, 3, 3);
	
	SudokoValidator t("success.txt");
	//t.Validate();
	
	SudokoValidator f ("fail.txt");
	//f.Validate();
	cout << "Checking success.txt, this sudoko should pass the validator" << endl;
	t.MulThread_Validate();

	cout << "Checking fail.txt, this sudoko should not pass the validator" << endl;
	f.MulThread_Validate();

  //  printf("hello from qu_lab2_SudukoValidator!\n");
    return 0;
}