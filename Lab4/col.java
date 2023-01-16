import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

class Value {
    public int value;
    Value(int value) { this.value = value; }
}

public class col {
    public static void main(String[] args) {
	ArrayList<Value> values = new ArrayList<Value>();

	for (int i = 0; i < 10; i++) 
	    values.add(new Value(i));
	for (var i : values)
	    System.out.print(i.value);

	Collections.sort(values, new Comparator<Value>(){
		public int compare(Value val1, Value val2) {
		    return val2.value - val1.value;
		}
	    });

	System.out.println("");

	for (var i : values)
	    System.out.print(i.value);
    }
}
