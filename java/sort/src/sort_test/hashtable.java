package sort_test;

public class hashtable {
	
	private static hashtable t;
	
	public static hashtable GetInstance()
	{
		if (t == null) 
			t = new hashtable();
		return t;
	}
	
	public void print()
	{
		System.out.println("i am in hashtable");
	}
	
	private hashtable()
	{
		
	}
}
