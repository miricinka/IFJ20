//compiler 3
//
package main

func main(){
	a := 1
	b := "str"
	c := 4.6
	a, b, c = 2, "stringik", 6.7
	a, a = foo(_)
}

func foo(s int)(int){
	return 1
}