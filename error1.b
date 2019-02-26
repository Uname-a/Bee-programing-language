x;

x == if[ 1 < 2 ? true ],

println("x is ", x),

x == if[ 0 = 1 ? 10 : else 20 ] + 5,    

println("x is ", x)

# comment test
x == if[ x < 0 ? "first case": x = 0 ?"second case": x > 0 ?"third case": else  "impossible"],

println("x is ", x),

## comment test 2
x == if[ x = "third case" ?
         y := 2 :;
         y == y + 2,
         x == y,# inline comment test #
         x * y                          
       : else
         "false"
       ],

println("x is ", x),
