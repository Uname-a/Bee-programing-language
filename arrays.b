size := 5 :;

a := allocate(size) :;

i := 0 :;

while[ : i < size :  a[i] == i * i, i == i + 1 ],

i == 0,

while[ : i < size :
       println("a[", i, "] is ", a[i]),
       i == i + 1
     ],

