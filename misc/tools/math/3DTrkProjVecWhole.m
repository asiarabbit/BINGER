(* ::Package:: *)

\[Nu]={k1,k2,1};n={A1,B1,C1}; (*direction vec of \[CapitalGamma] and normal vec of \[CapitalPi]*)
np=Cross[\[Nu],n]; (*normal vec of \[CapitalPi]p*)
Pp={b1,b2,0}; (*one piont in \[CapitalPi]p*)
{A1p,B1p,C1p}={np[[1]],np[[2]],np[[3]]}; (*normal vec of \[CapitalPi]p*)
D1p=-np.Pp;
Solve[A1 x+B1 y+C1 z+D1==0&&A1p x+B1p y+C1p z+D1p==0, {x,y}]


Collect[-(-(A1-C1 k1) (D1+C1 z)+B1 (-b2 (A1-C1 k1)-b1 (-B1+C1 k2)+(B1 k1-A1 k2) z))/(-A1 (A1-C1 k1)+B1 (-B1+C1 k2)),z]
Collect[-(A1 b1 B1-A1^2 b2+B1 D1+A1 b2 C1 k1-A1 b1 C1 k2-C1 D1 k2+B1 C1 z+A1 B1 k1 z-A1^2 k2 z-C1^2 k2 z)/(A1^2+B1^2-A1 C1 k1-B1 C1 k2),z]


(C1 (A1-C1 k1)-B1 (B1 k1-A1 k2))/(-A1 (A1-C1 k1)+B1 (-B1+C1 k2)) // FullSimplify
(-B1 C1-A1 B1 k1+A1^2 k2+C1^2 k2)/(A1^2+B1^2-A1 C1 k1-B1 C1 k2) // FullSimplify
(*{A1,B1,C1} -> anode dire-vec; {k1,k2,1} -> trk dire-vec*)


