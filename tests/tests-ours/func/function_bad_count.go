//compiler 6
//
package main

func main(){
	a := 1
	b := "str"
	c := 4.6
	a, b, c = 2, "stringik", 6.7
	a, a = foo()
}

func foo()(int){
	return 1
}