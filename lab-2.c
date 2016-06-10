# include <iostream.h>
# include <stdio.h>
int Available[50],Allocation[100][50],Max[100][50];
int Need[100][50],Request[40],Work[50],Finish[100],p[100];
int i,j,n,m,l=0,flag=0;
void safe()
{	
	for (i=0; i<n; )
		{
			if (Finish[i]==0)
			{
				flag=0;
				for (j=0; j<m; j++)
				{
					if (Work[j]>=Need[i][j])
						flag=flag+1;
				}
		
				if(flag==m)
				{
					p[l]=i; 
					Finish[i]=1;
					for (j=0; j<m;j++)
					{
						Work[j]=Work[j]+Allocation[i][j];
					}
					l=l+1;
					i= -1;
				}
			}
			i++;
		}
}

int main()
{
	int k=0,r=0;
	char b;


	//输入有关数组的值
	cout<<"请输入进程数目:"<<endl;
    cin>>n;
    cout<<"请输入资源种类数:"<<endl;
	cin>>m;
	cout<<"请输入各类可利用资源的数量Available["<<m<<"]:"<<endl;
    for (j=0; j<m; j++)
	{
		cin>>Available[j];
		Work[j]=Available[j];          //初始化Work[j]
	}

	cout<<"\n请输入各进程当前已分配的资源数量Allocation["<<n<<"]["<<m<<"]:"<<endl;
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			cin>>Allocation[i][j];
		}
		Finish[i]=0;                   //初始化Finish[i]
	}

	cout<<"\n请输入各进程对各类资源的最大需求数Max["<<n<<"]["<<m<<"]:"<<endl;
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			cin>>Max[i][j];
			Need[i][j] = Max[i][j]-Allocation[i][j];
		}
	}

	//判断当前状态的安全性
	safe();
	if (l<n)
	{
		cout<<"\n当前状态不安全";
		return 0;
	}
	else
	{
		l=0;
		cout<<"\n当前状态安全，安全序列为:"<<endl;
		for (i=0; i<n; i++)
		{
			cout<<"->p"<<p[i];
			Finish[i]=0;
		}
		cout<<endl;
	}
	
    do
	{
		//输入请求
		cout<<"\n请输入申请资源的进程编号(0,1…,n-1):"<<endl;
		cin>>k;
		cout<<"\n请输入申请各类资源的数量:"<<endl;
		for (j=0; j<m; j++)
		{
			cout<<"Request"<<k<<"["<<j<<"]:";
			cin>>Request[j];
		}

		//判断输入的情况
		for (j=0; j<m; j++)
		{
			if (Request[j] > Need[k][j])
			{
			cout<<"出错！申请大于需要量"<<endl;
			return 0;
			}
		}
		for (j=0; j<m; j++)
		{
			if (Request[j]>Available[j])
			{
				cout<<"无足够资源"<<endl;
				return 0;
			}
		}

		//改变Avilable、allocation、Need的值
		for (j=0; j<m; j++)
		{
			Available[j] = Available[j]-Request[j];
			Allocation[k][j] = Allocation[k][j]+Request[j];
			Need[k][j] = Need[k][j]-Request[j];
			Work[j] = Available[j];
		}

		//判断当前状态的安全性
		safe();
		if (l<n)
		{
			l=0;
			cout<<"\n当前状态不安全";
			//恢复数据 
			for (j=0; j<m; j++)
			{
				Available[j] = Available[j]+Request[j];
				Allocation[k][j] = Allocation[k][j]-Request[j];
				Need[k][j] = Need[k][j]+Request[j];
				Work[j] = Available[j];
			}
			for (i=0; i<n; i++)
				Finish[i]=0;
		}
		else
		{
			l=0;
			cout<<"\n当前状态安全，安全序列为:"<<endl;
			for (i=0; i<n; i++)
			{
				cout<<"->p"<<p[i];
				Finish[i]=0;
			}
			cout<<endl;
		}

        //显示输入各数组的值
		cout<<"\n当前各数组的值如下："<<endl;
	    cout<<"Available[m]:"<<endl;
        for (j=0; j<m; j++)
		{
			cout<<Available[j]<<"    ";
		}
	    cout<<endl;
	    cout<<"\nAllocation[n][m]:"<<endl;
	    for (i=0; i<n; i++)
		{
			for (j=0; j<m; j++)
			{
				cout<<Allocation[i][j]<<"    "; 
			}
		    cout<<endl;
		}
	    cout<<"\nMax[n][m]:"<<endl;
	    for (i=0; i<n; i++)
		{
			for (j=0; j<m; j++)
			{
				cout<<Max[i][j]<<"    ";
			}
		    cout<<endl;
		}

        cout<<"\n要继续申请资源，请按'y'或'Y'，否则按'n'或'N'"<<endl;
		cin>>b;
		if(b=='y'||b=='Y')
		{
			r=1;
			//输入请求
		    cout<<"\n请输入申请资源的进程编号(0,1…,n-1):"<<endl;
		    cin>>k;
		    cout<<"\n请输入申请各类资源的数量:"<<endl;
		    for (j=0; j<m; j++)
			{
				cout<<"Request"<<k<<"["<<j<<"]:";
				cin>>Request[j];
			}
		}
		else
		r=0;		
	}while (r==1);
	
	return 0;
}