package sort_test;

public class singlelinked {
	public class node {
		private int value;
		private node next;
		node(int v)	{
			value = v;
		}
		int GetValue() {
			return value;
		}
	}
	
	private node head;
	private node tail;
	
	public void AddNode(int value) {
		node n = new node(value);
		if (head == null)
		{
			head = n;
			tail = n;
		}
		else
		{
			tail.next = n;
			tail = n;
		}
	}
	
	public void PrintList() {
		node n = head;
		while (n != null) {
			System.out.print(n.value + ",");
			n = n.next;
		}
		System.out.println();
	}
	
	public void ReverseList() {
		//head = ReverseList_Internal_trans(head);
		ReverseList_Internal(head);
		head = tail;
		System.out.println("hello");
	}
	
	private node ReverseList_Internal(node n) {
		if (n == null || n.next == null)
		{
			return n;
		}
		else
		{
			node nn = ReverseList_Internal(n.next);
			nn.next = n;
			n.next = null;
			return n;
		}
	}
	
	private node ReverseList_Internal_trans(node n) {
		node prev = null;
		node curr = null;
		node next = null;
		
		prev = n;
		if (n != null)
		{
			curr = n.next;
			n.next = null;
		}
		
		while (curr != null)
		{
			next = curr.next;
			curr.next = prev;
			prev = curr;
			curr = next;
		}
		
		return prev;
	}
	
	public Iterator GetIterator() {
		return new Iterator();
	}
	
	public class Iterator
	{
		private node current;
		
		public node Next()
		{
			if (current != null)
			{
				current = current.next;
				return current;
			}
			return null;
		}
		
		public node GetNode()
		{
			return current;
		}
		
		public Iterator() {
			current = head;
		}
	}
}
