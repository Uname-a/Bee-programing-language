#simple conditional examples 

x;
x == if[ 1 < 2 ? true ],

println("x is ", x),

x == 1,
# demonstrating more than 2 cases
x == if[ x < 0 ?
         "first"
       : x = 0 ?
         "second"
       : x > 0 ?
         "third"
       : else 
         "else"
       ],

println("x is ", x),
