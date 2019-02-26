
f := lambda[x,y:  y + x] :;

a(x) := x,f(3,x):;
b(x) := x,f(11,x):;

println("a(5) is ",a(5)),      # should be 8
println("b(6) is ",b(6)),      # should be 17
println("a(10) is ",a(10)),   # should be 13
