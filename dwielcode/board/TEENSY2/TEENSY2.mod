PCBNEW-LibModule-V1  Sat 19 May 2012 11:18:08 AM EDT
# encoding utf-8
Units deci-mils
$INDEX
DPAK2
DPAK2-GDS
R3-HALF
SIL-4-5mm
SIL-5-5mm
SIL-6-5mm
SW_PUSH_6MM
TEENSY2
TO220GDS_VERT
$EndINDEX
$MODULE DPAK2
Po 0 0 0 15 4FB15C72 00000000 ~~
Li DPAK2
Cd MOS boitier DPACK G-D-S
Kw CMD DPACK
Sc 0
AR /4FB14AB9
Op 0 0 0
At SMD
T0 0 -4100 500 400 0 80 N V 21 N "Q1"
T1 0 -950 400 400 0 80 N V 21 N "RFD16N05L"
DS 550 -600 550 650 50 21
DS 550 650 1250 650 50 21
DS 1250 650 1250 -600 50 21
DS -1250 -600 -1250 650 50 21
DS -1250 650 -550 650 50 21
DS -550 650 -550 -600 50 21
DS 1350 -3000 1350 -600 50 21
DS 1350 -600 -1350 -600 50 21
DS -1350 -600 -1350 -3700 50 21
DS -1350 -3750 1350 -3750 50 21
DS 1350 -3700 1350 -3000 50 21
$PAD
Sh "G" R 650 1200 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po -900 0
$EndPAD
$PAD
Sh "D" R 2400 2400 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 0 -2499
$EndPAD
$PAD
Sh "S" R 650 1200 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 900 0
$EndPAD
$SHAPE3D
Na "smd/dpack_2.wrl"
Sc 1 1 1
Of 0 0 0
Ro 0 0 0
$EndSHAPE3D
$EndMODULE DPAK2
$MODULE DPAK2-GDS
Po 0 0 0 15 4FB15C72 00000000 ~~
Li DPAK2-GDS
Cd MOS boitier DPACK G-D-S
Kw CMD DPACK
Sc 0
AR /4FB14AB9
Op 0 0 0
At SMD
T0 0 -4100 500 400 0 80 N V 21 N "Q1"
T1 0 -950 400 400 0 80 N V 21 N "RFD16N05L"
DS 550 -600 550 650 50 21
DS 550 650 1250 650 50 21
DS 1250 650 1250 -600 50 21
DS -1250 -600 -1250 650 50 21
DS -1250 650 -550 650 50 21
DS -550 650 -550 -600 50 21
DS 1350 -3000 1350 -600 50 21
DS 1350 -600 -1350 -600 50 21
DS -1350 -600 -1350 -3700 50 21
DS -1350 -3750 1350 -3750 50 21
DS 1350 -3700 1350 -3000 50 21
$PAD
Sh "G" R 650 1200 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po -900 0
$EndPAD
$PAD
Sh "D" R 2400 2400 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 0 -2499
$EndPAD
$PAD
Sh "S" R 650 1200 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 900 0
$EndPAD
$SHAPE3D
Na "smd/dpack_2.wrl"
Sc 1 1 1
Of 0 0 0
Ro 0 0 0
$EndSHAPE3D
$EndMODULE DPAK2-GDS
$MODULE R3-HALF
Po 0 0 0 15 4FB505E4 00000000 F~
Li R3-HALF
Cd Resitance 3 pas
Kw R
Sc 0
AR /4FB14AAA
Op 0 A 0
T0 0 50 550 500 0 80 N I 21 N "R1"
T1 0 50 550 500 0 80 N V 21 N "44.9K"
DS -1000 0 -800 0 80 21
DS 1000 0 800 0 80 21
DS 800 0 800 -400 80 21
DS 800 -400 -800 -400 80 21
DS -800 -400 -800 400 80 21
DS -800 400 800 400 80 21
DS 800 400 800 0 80 21
DS -800 -200 -600 -400 80 21
$PAD
Sh "1" C 550 550 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 2 "N-000024"
Po -1000 0
$EndPAD
$PAD
Sh "2" C 550 550 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 1 "N-000001"
Po 1000 0
$EndPAD
$SHAPE3D
Na "discret/resistor.wrl"
Sc 0.3 0.3 0.3
Of 0 0 0
Ro 0 0 0
$EndSHAPE3D
$EndMODULE R3-HALF
$MODULE SIL-4-5mm
Po 0 0 0 15 4FB5BAAA 00000000 ~~
Li SIL-4-5mm
Cd Connecteur 5 pins
Kw CONN DEV
Sc 0
AR 
Op 0 0 0
T0 -250 -1000 680 427 0 107 N V 21 N "SIL-4"
T1 0 -1000 600 400 0 120 N I 21 N "Val**"
DS 4200 1650 -4200 1650 149 21
DS -4200 1650 -4200 -1350 149 21
DS -4200 -1350 4200 -1350 149 21
DS 4200 -1350 4200 1650 149 21
DS -4200 1650 -4200 -1350 149 21
DS -4200 150 -4200 -1350 149 21
DS -4200 -1350 -4200 1650 149 21
$PAD
Sh "1" R 550 550 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -3000 0
$EndPAD
$PAD
Sh "2" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1000 0
$EndPAD
$PAD
Sh "3" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 1000 0
$EndPAD
$PAD
Sh "4" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 3000 0
$EndPAD
$EndMODULE SIL-4-5mm
$MODULE SIL-5-5mm
Po 0 0 0 15 4FB5B98E 00000000 ~~
Li SIL-5-5mm
Cd Connecteur 5 pins
Kw CONN DEV
Sc 0
AR 
Op 0 0 0
T0 -250 -1000 681 428 0 120 N V 21 N "SIL-5"
T1 0 -1000 600 400 0 120 N I 21 N "Val**"
DS 5200 -1350 5200 1650 149 21
DS 5200 1650 -5200 1650 149 21
DS -5200 1650 -5200 -1350 149 21
DS -5200 -1350 5200 -1350 149 21
DS -5200 150 -5200 -1350 149 21
DS -5200 -1350 -5200 1650 149 21
$PAD
Sh "1" R 550 550 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -4000 0
$EndPAD
$PAD
Sh "2" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -2000 0
$EndPAD
$PAD
Sh "3" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 0 0
$EndPAD
$PAD
Sh "4" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 2000 0
$EndPAD
$PAD
Sh "5" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 4000 0
$EndPAD
$EndMODULE SIL-5-5mm
$MODULE SIL-6-5mm
Po 0 0 0 15 4FB5BA36 00000000 ~~
Li SIL-6-5mm
Cd Connecteur 5 pins
Kw CONN DEV
Sc 0
AR 
Op 0 0 0
T0 -250 -1000 680 427 0 107 N V 21 N "SIL-6"
T1 0 -1000 600 400 0 120 N I 21 N "Val**"
DS 5200 1650 6200 1650 149 21
DS 6200 1650 6200 -1350 149 21
DS 6200 -1350 5200 -1350 149 21
DS -6200 -1350 -6200 1650 149 21
DS -6200 1650 -5200 1650 149 21
DS -6200 -1350 -5150 -1350 149 21
DS 5200 1650 -5200 1650 149 21
DS -5200 -1350 5200 -1350 149 21
$PAD
Sh "1" R 550 550 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -5000 0
$EndPAD
$PAD
Sh "2" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -3000 0
$EndPAD
$PAD
Sh "3" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1000 0
$EndPAD
$PAD
Sh "4" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 1000 0
$EndPAD
$PAD
Sh "5" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 3000 0
$EndPAD
$PAD
Sh "6" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5000 0
$EndPAD
$EndMODULE SIL-6-5mm
$MODULE SW_PUSH_6MM
Po 0 0 0 15 4FB7B97D 00000000 ~~
Li SW_PUSH_6MM
Cd Pushbutton 6mm OMRON B3F-1000
Sc 0
AR /4BB0044D
Op 0 0 0
T0 0 50 400 400 0 80 N V 21 N "SW1"
T1 0 650 400 400 0 80 N I 21 N "SW_PUSH"
DS -1200 -1200 1200 -1200 149 21
DS 1200 -1200 1200 1200 149 21
DS 1200 1200 -1200 1200 149 21
DS -1200 1200 -1200 -1200 149 21
$PAD
Sh "1" C 599 599 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1000 0
$EndPAD
$PAD
Sh "2" C 599 599 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 1000 0
$EndPAD
$EndMODULE SW_PUSH_6MM
$MODULE TEENSY2
Po 0 0 0 15 4FB64DAB 00000000 ~~
Li TEENSY2
Cd 28 pins DIL package, round pads, width 600mil
Kw DIL
Sc 0
AR 
Op 0 0 0
T0 -4500 -1500 700 450 0 113 N V 21 N "TEENSY2"
T1 2000 1000 700 650 0 120 N V 21 N "Val**"
DS -6500 -500 -6000 -500 150 21
DS -6000 -500 -6000 500 150 21
DS -6000 500 -6500 500 150 21
DS -6500 -2500 6500 -2500 150 21
DS 6500 -2500 6500 2500 150 21
DS 6500 2500 -6500 2500 150 21
DS -6500 2500 -6500 -2500 150 21
$PAD
Sh "1" R 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -5500 2998
$EndPAD
$PAD
Sh "2" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -4500 2998
$EndPAD
$PAD
Sh "3" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -3498 2998
$EndPAD
$PAD
Sh "4" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -2499 2998
$EndPAD
$PAD
Sh "5" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1499 2999
$EndPAD
$PAD
Sh "6" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -500 2999
$EndPAD
$PAD
Sh "7" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 500 2999
$EndPAD
$PAD
Sh "8" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 1499 2999
$EndPAD
$PAD
Sh "9" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 2499 2999
$EndPAD
$PAD
Sh "10" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 3499 2999
$EndPAD
$PAD
Sh "11" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 4500 2999
$EndPAD
$PAD
Sh "12" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 2999
$EndPAD
$PAD
Sh "13" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 -2999
$EndPAD
$PAD
Sh "14" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 4500 -2999
$EndPAD
$PAD
Sh "15" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 3499 -2999
$EndPAD
$PAD
Sh "16" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 2499 -2999
$EndPAD
$PAD
Sh "17" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 1499 -2999
$EndPAD
$PAD
Sh "18" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 500 -2999
$EndPAD
$PAD
Sh "19" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -500 -2998
$EndPAD
$PAD
Sh "20" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1499 -2999
$EndPAD
$PAD
Sh "21" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -2499 -2999
$EndPAD
$PAD
Sh "22" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -3498 -2998
$EndPAD
$PAD
Sh "23" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -4500 -2999
$EndPAD
$PAD
Sh "24" C 550 550 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -5500 -2999
$EndPAD
$PAD
Sh "25" C 599 599 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 2000
$EndPAD
$PAD
Sh "26" C 599 599 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 1000
$EndPAD
$PAD
Sh "27" C 599 599 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 0
$EndPAD
$PAD
Sh "28" C 599 599 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 -1000
$EndPAD
$PAD
Sh "29" C 599 599 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 5500 -2000
$EndPAD
$PAD
Sh "31" C 599 599 0 0 0
Dr 320 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1499 2000
$EndPAD
$PAD
Sh "30" C 599 599 0 0 0
Dr 318 0 0
At STD N 00E0FFFF
Ne 0 ""
Po -1499 -2000
$EndPAD
$SHAPE3D
Na "dil/dil_24-w600.wrl"
Sc 1 1 1
Of 0 0 0
Ro 0 0 0
$EndSHAPE3D
$EndMODULE TEENSY2
$MODULE TO220GDS_VERT
Po 0 0 0 15 4FB47966 00000000 ~~
Li TO220GDS_VERT
Cd Regulateur TO220 series LM78xx
Kw TR TO220
Sc 0
AR 
Op 0 0 0
T0 -1250 0 599 400 900 80 N V 21 N "TO220GDS_VERT"
T1 250 -2500 600 400 0 80 N V 21 N "VAL**"
DS 750 -2000 1000 -2000 150 21
DS 1000 -2000 1000 2000 150 21
DS 1000 2000 750 2000 150 21
DS -750 -2000 750 -2000 150 21
DS 750 -2000 750 2000 150 21
DS 750 2000 -750 2000 150 21
DS -750 2000 -750 -2000 150 21
$PAD
Sh "S" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 0 -1000
$EndPAD
$PAD
Sh "D" C 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 0 0
$EndPAD
$PAD
Sh "G" R 700 700 0 0 0
Dr 400 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 0 1000
$EndPAD
$SHAPE3D
Na "discret/to220_vert.wrl"
Sc 1 1 1
Of 0 0 0
Ro 0 0 0
$EndSHAPE3D
$EndMODULE TO220GDS_VERT
$EndLIBRARY
