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


	//�����й������ֵ
	cout<<"�����������Ŀ:"<<endl;
    cin>>n;
    cout<<"��������Դ������:"<<endl;
	cin>>m;
	cout<<"����������������Դ������Available["<<m<<"]:"<<endl;
    for (j=0; j<m; j++)
	{
		cin>>Available[j];
		Work[j]=Available[j];          //��ʼ��Work[j]
	}

	cout<<"\n����������̵�ǰ�ѷ������Դ����Allocation["<<n<<"]["<<m<<"]:"<<endl;
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			cin>>Allocation[i][j];
		}
		Finish[i]=0;                   //��ʼ��Finish[i]
	}

	cout<<"\n����������̶Ը�����Դ�����������Max["<<n<<"]["<<m<<"]:"<<endl;
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			cin>>Max[i][j];
			Need[i][j] = Max[i][j]-Allocation[i][j];
		}
	}

	//�жϵ�ǰ״̬�İ�ȫ��
	safe();
	if (l<n)
	{
		cout<<"\n��ǰ״̬����ȫ";
		return 0;
	}
	else
	{
		l=0;
		cout<<"\n��ǰ״̬��ȫ����ȫ����Ϊ:"<<endl;
		for (i=0; i<n; i++)
		{
			cout<<"->p"<<p[i];
			Finish[i]=0;
		}
		cout<<endl;
	}
	
    do
	{
		//��������
		cout<<"\n������������Դ�Ľ��̱��(0,1��,n-1):"<<endl;
		cin>>k;
		cout<<"\n���������������Դ������:"<<endl;
		for (j=0; j<m; j++)
		{
			cout<<"Request"<<k<<"["<<j<<"]:";
			cin>>Request[j];
		}

		//�ж���������
		for (j=0; j<m; j++)
		{
			if (Request[j] > Need[k][j])
			{
			cout<<"�������������Ҫ��"<<endl;
			return 0;
			}
		}
		for (j=0; j<m; j++)
		{
			if (Request[j]>Available[j])
			{
				cout<<"���㹻��Դ"<<endl;
				return 0;
			}
		}

		//�ı�Avilable��allocation��Need��ֵ
		for (j=0; j<m; j++)
		{
			Available[j] = Available[j]-Request[j];
			Allocation[k][j] = Allocation[k][j]+Request[j];
			Need[k][j] = Need[k][j]-Request[j];
			Work[j] = Available[j];
		}

		//�жϵ�ǰ״̬�İ�ȫ��
		safe();
		if (l<n)
		{
			l=0;
			cout<<"\n��ǰ״̬����ȫ";
			//�ָ����� 
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
			cout<<"\n��ǰ״̬��ȫ����ȫ����Ϊ:"<<endl;
			for (i=0; i<n; i++)
			{
				cout<<"->p"<<p[i];
				Finish[i]=0;
			}
			cout<<endl;
		}

        //��ʾ����������ֵ
		cout<<"\n��ǰ�������ֵ���£�"<<endl;
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

        cout<<"\nҪ����������Դ���밴'y'��'Y'������'n'��'N'"<<endl;
		cin>>b;
		if(b=='y'||b=='Y')
		{
			r=1;
			//��������
		    cout<<"\n������������Դ�Ľ��̱��(0,1��,n-1):"<<endl;
		    cin>>k;
		    cout<<"\n���������������Դ������:"<<endl;
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