//compiler 6
//

package main

func f(a int, b int)(int, string, float64){
	c := 4.6
	return a, b, c
}

func main()(){
	a := 12
	b := "ahoj"
	c := 4.6
	a, b, c = f(42, 44)
}