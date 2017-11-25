(* ::Package:: *)

(*math for XYUV transformation*)
Rx[\[Alpha]_]:={{1,0,0},{0,Cos[\[Alpha]],-Sin[\[Alpha]]},{0,Sin[\[Alpha]],Cos[\[Alpha]]}};
Ry[\[Alpha]_]:={{Cos[\[Alpha]],0,Sin[\[Alpha]]},{0,1,0},{-Sin[\[Alpha]],0,Cos[\[Alpha]]}};
Rz[\[Alpha]_]:={{Cos[\[Alpha]],-Sin[\[Alpha]],0},{Sin[\[Alpha]],Cos[\[Alpha]],0},{0,0,1}};
Ru[\[Phi],\[Theta]]=Ry[\[Phi]].Rz[\[Theta]]; (*(Rz[-\[Theta]].Ry[-\[Phi]])^-1*)
lu={xu,yu,zu}; (*the line projection in Oxu-yu-zu coordinate*)
lxy=Ru[\[Phi],\[Theta]].lu;
Eliminate[x==lxy[[1]] && y==lxy[[2]] && z==lxy[[3]]&&xu==ku zu+bu,{xu,yu,zu}] // FullSimplify
Collect[x Cos[\[Theta]] Cos[\[Phi]]+y Sin[\[Theta]] - (bu+ku z Cos[\[Phi]]+(ku x+z Cos[\[Theta]]) Sin[\[Phi]]),{x,y,z}]
(*function of projecting plane in the Oxyz coordinate*)
ff[\[Theta]_,\[Phi]_,ku_,bu_]:=x(Cos[\[Phi]] Cos[\[Theta]]-ku Sin[\[Phi]])+y Sin[\[Theta]]-z (ku Cos[\[Phi]] + Sin[\[Phi]]Cos[\[Theta]])-bu;
(*solve the intersection line of the U-V projecting planes*)
Solve[Eliminate[{ff[-Pi/6,\[Phi],ku,bu]==0,ff[Pi/6,\[Phi],kv,bv]==0},y],x] // FullSimplify
Solve[Eliminate[{ff[-Pi/6,\[Phi],ku,bu]==0,ff[Pi/6,\[Phi],kv,bv]==0},x],y] // FullSimplify


(*the above solved function of lx: x=k1t z+b1t and ly: y=k2t z+b2t*)
k1t=((ku+kv) Cos[\[Phi]]+Sqrt[3] Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);
b1t=(bu+bv)/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);
k2t=(Sqrt[3] (-ku+kv))/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);
b2t=(Sqrt[3] (-bu+bv) Cos[\[Phi]]+(-2 bv ku+2 bu kv) Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]);


(*express {ku, kv, bu, bv} in {k1t, k2t, b1t, b2t}*)
Solve[k1T==((ku+kv) Cos[\[Phi]]+Sqrt[3] Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]])&&
b1T==(bu+bv)/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]])&&
k2T==(Sqrt[3] (-ku+kv))/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]])&&
b2T==(Sqrt[3] (-bu+bv) Cos[\[Phi]]+(-2 bv ku+2 bu kv) Sin[\[Phi]])/(Sqrt[3] Cos[\[Phi]]-(ku+kv) Sin[\[Phi]]),{ku,kv,bu,bv}] // FullSimplify


Rx[\[Theta]_]:={{1,0,0},{0,Cos[\[Theta]],-Sin[\[Theta]]},{0,Sin[\[Theta]],Cos[\[Theta]]}};
Ry[\[Theta]_]:={{Cos[\[Theta]],0,Sin[\[Theta]]},{0,1,0},{-Sin[\[Theta]],0,Cos[\[Theta]]}}; (*note that z-x, not x-z, so the matrix is odd*)
Rz[\[Theta]_]:={{Cos[\[Theta]],-Sin[\[Theta]],0},{Sin[\[Theta]],Cos[\[Theta]],0},{0,0,1}};
R[\[Phi]_,\[Theta]_,\[Psi]_]:=Rz[\[Psi]].Rx[\[Theta]].Ry[\[Phi]]; (*\[Phi] goes first*)
R[\[Phi],\[Theta],\[Psi]] // MatrixForm
R[\[Phi],0,0] // MatrixForm;
R0[\[Phi]_,\[Theta]_,\[Psi]_]:=Ry[\[Psi]].Rz[\[Theta]].Ry[\[Phi]]; (*\[Phi] goes first*)
R0[\[Phi],\[Theta],\[Psi]] // MatrixForm;
r={353.,375,-85.9};
R[0.398282,0,0].r



