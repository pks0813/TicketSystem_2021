#include<iostream>
#include<string>
 
using namespace std;
int main()
{
	string t,ans,ans2;
	int i;
	freopen("10.out","r",stdin);
	char c;
	while(scanf("%c",&c)!=EOF) ans+=c;
	fclose(stdin);
	freopen("ans10.out","r",stdin);
	while(scanf("%c",&c)!=EOF) ans2+=c;
	fclose(stdin);
    printf("%d %d\n",ans.size(),ans2.size());
	if(ans.size()!=ans2.size()){cout<<"NO\n";return 0;}
	for(i=0;i<ans.size();i++)
		if(ans[i]!=ans2[i]){cout<<"NO\n";return 0;}
	cout<<"YES\n";
	return 0;
}