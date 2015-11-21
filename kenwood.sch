v 20110115 2
C 40000 40000 0 0 0 title-B.sym
B 49700 44200 1400 1700 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
P 51600 45400 51100 45400 1 0 0
{
T 51600 45400 5 10 0 0 0 0 1
pintype=out
T 51045 45395 5 10 1 1 0 6 1
pinlabel=RESET
T 51195 45445 5 10 1 1 0 0 1
pinnumber=1
T 51600 45400 5 10 0 0 0 0 1
pinseq=2
}
P 50400 46400 50400 45900 1 0 0
{
T 50400 46400 5 10 0 0 0 0 1
pintype=pwr
T 50155 45800 5 10 1 1 180 6 1
pinlabel=Vcc
T 50350 45995 5 10 1 1 90 0 1
pinnumber=5
T 50400 46400 5 10 0 0 0 0 1
pinseq=1
}
P 51600 44500 51100 44500 1 0 0
{
T 51600 44500 5 10 0 0 0 0 1
pintype=in
T 51045 44495 5 10 1 1 0 6 1
pinlabel=WDI
T 51195 44545 5 10 1 1 0 0 1
pinnumber=4
T 51600 44500 5 10 0 0 0 0 1
pinseq=3
}
P 49200 44500 49700 44500 1 0 0
{
T 49200 44500 5 10 0 0 0 0 1
pintype=in
T 49755 44495 5 10 1 1 0 0 1
pinlabel=MR
T 49605 44545 5 10 1 1 0 6 1
pinnumber=3
T 49200 44500 5 10 0 0 0 0 1
pinseq=5
}
P 50400 43700 50400 44200 1 0 0
{
T 50400 43700 5 10 0 0 0 0 1
pintype=pwr
T 50545 44400 5 10 1 1 180 0 1
pinlabel=GND
T 50350 44105 5 10 1 1 90 6 1
pinnumber=2
T 50400 43700 5 10 0 0 0 0 1
pinseq=4
}
T 49500 46100 9 10 1 0 0 0 1
MAX823
N 51600 44500 52800 44500 4
C 51900 43400 1 90 0 resistor-2.sym
{
T 51550 43800 5 10 0 0 90 0 1
device=RESISTOR
T 51600 43600 5 10 1 1 90 0 1
refdes=10K
}
C 43900 49400 1 90 0 resistor-2.sym
{
T 43550 49800 5 10 0 0 90 0 1
device=RESISTOR
T 43600 49600 5 10 1 1 90 0 1
refdes=4K7
}
C 41900 48400 1 0 0 resistor-2.sym
{
T 42300 48750 5 10 0 0 0 0 1
device=RESISTOR
T 42200 48700 5 10 1 1 0 0 1
refdes=1K
}
C 50200 42900 1 0 0 gnd-3.sym
{
T 50400 42900 5 10 0 1 0 0 1
device=GND
}
T 52900 44500 9 10 1 0 0 0 1
ARDUINO:PB0 (DIGITAL IO 8)
N 50400 46600 50400 46400 4
N 51600 45400 52800 45400 4
T 52900 45400 9 10 1 0 0 0 1
ATMEGA:29 (RESET)
N 50200 50000 52200 50000 4
T 48900 50100 9 10 1 0 0 0 1
ARDUINO:PD6 (DIGITAL IO 6)
T 52300 50100 9 10 1 0 0 0 1
MCP2515:28 (RESET)
C 43200 48000 1 0 0 npn-3.sym
{
T 44100 48500 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 44100 48500 5 10 1 1 0 0 1
refdes=Q1
}
N 42800 48500 43200 48500 4
N 43800 49000 43800 49400 4
N 41900 48500 41700 48500 4
T 40100 48100 9 10 1 0 0 0 1
ARDUINO:PD3 (DIGITAL IO 3)
C 43600 47500 1 0 0 gnd-3.sym
{
T 43800 47500 5 10 0 1 0 0 1
device=GND
}
N 43800 48000 43800 47800 4
C 44000 50800 1 180 0 12V-plus-1.sym
N 43800 50400 43800 50300 4
N 44700 49200 43800 49200 4
T 44700 49300 9 10 1 0 0 0 1
CANBUS DB9:2 (KENWOOD REVERSE)
N 50200 49000 52200 49000 4
T 48900 49200 9 10 1 0 0 0 1
ARDUINO:PD7 (DIGITAL IO 7)
T 52200 49200 9 10 1 0 0 0 1
CANBUS DB9:9 (KENWOOD REMOTE INPUT)
C 51600 42900 1 0 0 gnd-3.sym
{
T 51800 42900 5 10 0 1 0 0 1
device=GND
}
N 50400 43200 50400 43700 4
N 51800 43200 51800 43400 4
N 51800 44300 51800 44500 4
C 43900 45900 1 90 0 resistor-2.sym
{
T 43550 46300 5 10 0 0 90 0 1
device=RESISTOR
T 43600 46100 5 10 1 1 90 0 1
refdes=4K7
}
C 41900 44900 1 0 0 resistor-2.sym
{
T 42300 45250 5 10 0 0 0 0 1
device=RESISTOR
T 42200 45200 5 10 1 1 0 0 1
refdes=1K
}
C 43200 44500 1 0 0 npn-3.sym
{
T 44100 45000 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 44100 45000 5 10 1 1 0 0 1
refdes=Q2
}
N 42800 45000 43200 45000 4
N 43800 45500 43800 45900 4
N 41900 45000 41700 45000 4
C 43600 44000 1 0 0 gnd-3.sym
{
T 43800 44000 5 10 0 1 0 0 1
device=GND
}
N 43800 44500 43800 44300 4
C 44000 47300 1 180 0 12V-plus-1.sym
N 43800 46900 43800 46800 4
N 44700 45700 43800 45700 4
T 40100 44600 9 10 1 0 0 0 1
ARDUINO:PD4 (DIGITAL IO 4)
T 44700 45800 9 10 1 0 0 0 1
CANBUS DB9:7 (KENWOOD PRK SW)
C 43900 42300 1 90 0 resistor-2.sym
{
T 43550 42700 5 10 0 0 90 0 1
device=RESISTOR
T 43600 42500 5 10 1 1 90 0 1
refdes=4K7
}
C 41900 41300 1 0 0 resistor-2.sym
{
T 42300 41650 5 10 0 0 0 0 1
device=RESISTOR
T 42200 41600 5 10 1 1 0 0 1
refdes=1K
}
C 43200 40900 1 0 0 npn-3.sym
{
T 44100 41400 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 44100 41400 5 10 1 1 0 0 1
refdes=Q3
}
N 42800 41400 43200 41400 4
N 43800 41900 43800 42300 4
N 41900 41400 41700 41400 4
C 43600 40400 1 0 0 gnd-3.sym
{
T 43800 40400 5 10 0 1 0 0 1
device=GND
}
N 43800 40900 43800 40700 4
C 44000 43700 1 180 0 12V-plus-1.sym
N 43800 43300 43800 43200 4
N 44700 42100 43800 42100 4
T 40100 41000 9 10 1 0 0 0 1
ARDUINO:PD5 (DIGITAL IO 5)
T 44700 42200 9 10 1 0 0 0 1
CANBUS DB9:8 (KENWOOD ILLUMI)
T 54000 40100 9 10 1 0 0 0 1
Vitaly Mayatskikh
T 54000 40400 9 10 1 0 0 0 1
2
T 50100 40700 9 10 1 0 0 0 3
Volvo steering wheel controls adapter for Kenwood

https://github.com/vtl/volvo-kenwood
T 50200 40100 9 10 1 0 0 0 1
1
T 52100 40100 9 10 1 0 0 0 1
1
C 50200 46600 1 0 0 5V-plus-1.sym
