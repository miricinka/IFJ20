//compiler 6
//

package main

func main()(){
	a:=2
	a = foo(42,43)
	return 45
}

func foo(a int, b int)(int){
	return 42
}