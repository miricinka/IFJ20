//compiler 3
//

package main

func main()(){
	a := 12
	f := "str"
	a = f(42, 44)
}


func f(a int, b int)(int){
	return 42
}
