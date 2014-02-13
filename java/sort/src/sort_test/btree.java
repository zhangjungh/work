package sort_test;


public class btree {
	static public class node {
		public int value;
		public node left;
		public node right;
		node(int v)
		{
			value = v;
		}
	}
	
	private static node root = null;
	
	
	public static void AddNode(int value)
	{
		node _n = new node(value);
		if (root == null)
			root = _n;
		else
		{
			node n = root;
			while (n != null)
			{
				if (n.value == value)
				{
					System.out.println("same value detected!");
					break;
				}
				else if (n.value > value)
				{
					if (n.left != null)
						n = n.left;
					else
					{
						n.left = _n;
						break;
					}
				}
				else
				{
					if (n.right != null)
						n = n.right;
					else 
					{
						n.right = _n;
						break;
					}
					
				}
			}			
		}
	}
	
	public static node SearchByValue(int value)
	{
		node n = root;
		
		while (n != null)
		{
			if (n.value == value)
			{
				break;
			}
			else if (n.value > value)
			{
				n = n.left;
			}
			else
			{
				n = n.right;
			}
		}
		
		return n;
	}
	
	public static boolean IsInTree(int value)
	{
		return SearchByValue(value) != null;
	}
	
	private static node FindPosition(int value)
	{
		node n = root;
		
		while (n != null)
		{
			if (n.value == value)
			{
				System.out.println("same value detected! " + value);
				return null;
			}
			else if (n.value > value)
			{
				if (n.left != null)
					n = n.left;
				else
					return n;
			}
			else
			{
				if (n.right != null)
					n = n.right;
				else 
					return n;
			}
		}
		
		return n;
	}
	
	public static void travelCenter()
	{
		travel(root);
	}
	
	private static void travel(node n)
	{
		if (n != null)
		{
			System.out.println(n.value);
			travel(n.left);
			travel(n.right);
		}
	}
	
}
