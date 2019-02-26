#simple comment #
square(x) := x * x :;
sqrt(x) := x ^ 0.5 :;
magnitude(v) := sqrt(square(v[0]) + square(v[1])) :;
v := allocate(2) :;
v[0] == 5, v[1] == 6,

println( "<> is ", magnitude(v), "."),

# classic
fib(n) :=
    if[ n < 2 ?
        n
      : else
        fib(n-1) + fib(n-2)
      ]
    :;

println("The 6th fibonacci number is ",, fib(6)),

if[ x > 2 ? y == 2: x < 2 ? y == 5 : else y == 0],
