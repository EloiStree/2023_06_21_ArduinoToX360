![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/d67d5e6d-2545-448d-a018-0c78ad29710c)  
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/5cb8c578-b97f-4355-bb95-27f2e2169928)  
If not change, the max size of a text is around 240 char  
  

```
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
```
And the max size of waiting command is 40.

Maxium is around 


```
release 3000> jrh%0.5 jrv%0.5  jlh%0.5 jlv%0.5  tl%0.5 tr%0.5 5000> release 2000> jrh%-0.5 jrv%-0.5  jlh%-0.5 jlv%-0.5  tl%-0.5 tr%-0.5 5000> release 2000> jrh%1  jrv%1  jlh%1   jlv%1   tl%1   tr%1 5000> release ;

jlh%0.0 jlv%0.5 2000> release; 
```

```
release jrh%-1 jrv%0.9 tl%1 tr%0.1  au.  ad. ar. al. 2000> release ad'  al'  au' ar' al' 5000> ad. 1000> ad'  1000> au.  1000> au'  1000> ar.  1000> ar'  1000> al.  1000> al' ;
```

```
//Test all button at same time
release a. b. y. x. 5000> a. b. y. x. release ;µ
release 4000> al.   ar. au. ad.   5000> al'   ar' au' ad' release ;

//Test all button at after each other
release 4000> a. 1000> a'  1000> b. 1000> b'  1000> y. 1000> y'  1000> x. 1000> x' release ;
release 4000> al. 1000> al'  1000> ar. 1000> ar'  1000> au. 1000> au'  1000> ad. 1000> ad'  1000> release ;


release 4000> ml. 1000> ml'  1000> mc. 1000> mc'  1000> mr. 1000> mr' release 5000>  ml. mc. mr. 4000> ml'  mc' mr' ;
// MC logo don't seem to be usable.  Maybe I did something wrong. Or it was not on 360 controller ?



release 4000> tl%0 1000> tl%0.2 1000> tl%0.5 1000>  tl%0.9 1000>  tl%1 3000> tr%0 1000> tr%0.2 1000> tr%0.5 1000>  tr%0.9 1000>  tr%1 5000> release ;


2000> al. y. b. 2000> al' y' b' release;
2000> jlh%1  jlv%0.1  jrh%1  jrv%0.1 8000> release ;

```


Diablo 4
```

jlh%0.4 jlv%-0.0  jrh%1 jrv%1 3000> release ; 3000> release ;

```


