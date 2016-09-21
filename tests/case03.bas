' case03.bas
' գործողություններ

' թվաբանական
FUNCTION Arithmetic(x, y)
  PRINT x + y
  PRINT x - y
  PRINT x * y
  PRINT x / y
  PRINT x ^ y
  PRINT y
  PRINT -x
  PRINT 3.14
  PRINT (x + y) * (x - y)
END FUNCTION

' համեմատման
FUNCTION Comparison(x, y)
  PRINT x = y
  PRINT x <> y
  PRINT x > y
  PRINT x >= y
  PRINT x < y
  PRINT x <= y
END FUNCTION

' տրամաբանական
FUNCTION Logical(x, y)
  PRINT x OR y
  PRINT x AND y
  PRINT NOT x
END FUNCTION

' ֆունկցիաների ստուգում
FUNCTION Main()
  CALL Arothmetical 1.2, 777
  CALL Comparison 18, -5
  CALL Logical 1, 0
END FUNCTION

