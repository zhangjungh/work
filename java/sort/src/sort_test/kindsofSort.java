package sort_test;

public class kindsofSort {
	static private int array[]={6,23,5,4,3,1,8,45,12,9,2,7};
	static private int length=array.length;
	
	public static void printArray(int[] a)
	{
		for (int i : a)
			System.out.print(i + ",");
		System.out.println();
	}
	
	public static void InsertSort(int[] a)
	{
		int i=1;
		for(;i<a.length;i++)
		{
			int key=a[i];
			int j=i-1;
			while (j>=0 && a[j] > key)
			{
				a[j+1] = a[j];
				--j;
			}
			a[j+1] = key;
			/*
			int temp = -1;
			for(int j=i-1;j>=0;j--)
			{
				if(key<a[j])
				{
					a[j+1]=a[j];
					temp = j;
				}
				else
				{
					break;
				}
			}
			if (temp != -1)
				a[temp] = key;
			//*/
			System.out.println("round "+i);
			printArray(a);
		}
		printArray(a);
	}
	
	public static void BubbleSort(int a[])
	{
		for(int i=1;i<a.length;i++)
		{
			for(int j=a.length-1;j>=i;j--)
			{
				if(a[j]<a[j-1])
				{
					int temp=a[j];
					a[j]=a[j-1];
					a[j-1]=temp;
				}
			}
			System.out.println("round "+i);
			printArray(a);
		}
		printArray(a);
	} 
	
	public static void QuickSort(int a[],int p,int r)
	{
		if(p<r)
		{
		int flag=a[r];
		int i=p-1;
		for(int j=i+1;j<r;j++)
		{
			if(a[j]<flag)
			{
				i++;
				int temp=a[j];
				a[j]=a[i];
				a[i]=temp;
			}
		}
		int temp=a[r];
		a[r]=a[i+1];
		a[i+1]=temp;
		//System.out.println("r="+r);
		//System.out.println("i="+i);
		QuickSort(a,p,i);
		QuickSort(a,i+2,r);
		printArray(a);
		}
	}
	
	public static void SelectSort(int a[])
	{
		for(int i=0;i<a.length;i++)
		{
			int temp=i;
			for(int j=i;j<a.length;j++)
			{
				if(a[temp]>a[j])
					temp=j;
			}  
			int _temp=a[i];
			a[i]=a[temp];
			a[temp]=_temp;
		}
		printArray(a);
	}
	
	public static void btreeTest()
	{
		for (int i=0; i<length; ++i)
		{
			btree.AddNode(array[i]);
		}
		btree.travelCenter();

		System.out.println(btree.IsInTree(7));
		System.out.println(btree.IsInTree(100));		
	}
	
	public static boolean binarySearch(int[] a, int value)
	{
		int h = 0;
		int e = a.length-1;
		int p = (h+e) / 2;
		while (h <= e && a[p] != value)
		{
			if (a[p] > value)
			{
				e=p-1;
			}
			else
			{
				h=p+1;
			}
			p = (h+e) / 2;
		}
		System.out.println(a[p] == value);
		return a[p] == value;
	}
	
	public static void BinarySearch_recrusive(int[] a,int value,int head,int foot)
	{
		if(head<=foot)
		{
			int flag=(head+foot)/2;
			if(a[flag]==value)
			{
				System.out.println("true");
			}
			else if(a[flag]>value)
			{
				BinarySearch_recrusive(a,value,head,flag-1);
			}
			else 
			{
				
				BinarySearch_recrusive(a,value,flag+1,foot);
			}
		}
		else 
			System.out.println("false");
		
	}
	
	public static void SingleLinkedTest() {
		singlelinked list = new singlelinked();
		for (int i=0; i<length; ++i) {
			list.AddNode(array[i]);
		}
		
		singlelinked.Iterator iter = list.GetIterator();
		
		while (iter.GetNode() != null)
		{
			System.out.print(iter.GetNode().GetValue() + ",");
			iter.Next();
		}
		
		list.PrintList();
		
		list.ReverseList();
		
		list.PrintList();
	}
	
	public static void main(String [] args) {
		
		printArray(array);
		//InsertSort(array);
		//BubbleSort(array);
		//QuickSort(array,0,length-1);
		//SelectSort(array);
		//binarySearch(array, 7);
		//binarySearch(array, 45);
		//binarySearch(array, 100);
		//for (int i=0; i<length; ++i)
			//BinarySearch_recrusive(array,array[i],0,length);
			//binarySearch(array, array[i]);
		
		hashtable t = hashtable.GetInstance();
		t.print();
		
		SingleLinkedTest();
		
	
	}

}
