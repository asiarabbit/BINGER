
//	s[0].x1 = x0 + k1*(z1-z0); // the denominator of the equation
	{
		// x1^3 + b x1^2 + c x1 + d = 0
		double b = -2.*b1 - x0 - k1*(z0+z1);
		double c = -x2*x2 + 2.*b1*(x0+x2) + 2.*k1*x2*z0 + z1*(2.*k1*x0-2.*z0+z1) + z2*(2.*z0-z2);
		double d = -2.*b1*(x0*x2-(z0-z1)*(z1-z2)) +
			x0*(x2*x2-2.*k1*x2*z1+(z1-z2)*(z1-z2)) - k1*(z0-z1)*(x2*x2-z1*z1+z2*z2);

		// let x1 = y - b/3, then y^3 + py + q = 0
		double p = c - b*b/3.;
		double q = d + 2.*b*b*b/27. - b*c/3.;


		// solve y^3 + py + q = 0 using Cardono's formula:
		double qh = q/2., pt = p/3.;
		cdouble Delta(qh*qh + pt*pt*pt, 0.);
		cdouble sqrtDelta = sqrt(Delta);
		cdouble omega(-0.5, Sqrt3()/2.);
		cdouble cqh(qh,0.);

		cdouble alphap = pow(-cqh + sqrtDelta, 1./3.);
		cdouble alpham = pow(-cqh - sqrtDelta, 1./3.);

		cdouble y[3];
		y[0] = alphap + alpham;
		y[1] = omega*alphap + omega*omega*alpham;
		y[2] = omega*omega*alphap + omega*alpham;

		// output the results
		for(int i = 0; i < 3; i++){
			if(fabs(y[i].imag()) < 1E-7)
				s[i].x1 = y[i].real() - b/3.;
			else s[i].x1 = -9999.;
		} // end for over i
	}
	for(int i = 0; i < 3; i++){
		if(fabs(s[i].x1) > 600.) s[i].x1 = -9999.; // out of Mag mouth (+-500)
		if(-9999. == s[i].x1) continue;
		s[i].ki = (s[i].x1 - x0)/(z1 - z0);
		if(fabs(s[i].ki) > 3.){ s[i].x1 = -9999.; continue; }
		s[i].bi = x0 - s[i].ki*z0;
		s[i].zh = (-b1 + s[i].bi)/(k1 - s[i].ki);
		s[i].xh = k1*s[i].zh + b1;
		s[i].dtheta = atan(k1) - atan(s[i].ki); // anti-clockwise deflection angle
		if(fabs(s[i].dtheta)/DEGREE() > 180.){ s[i].x1 = -9999.; continue; }
		// P1Ph or PhP2 (pdf in misc/tools/math/UniBSolu)
		s[i].dd2 = (z2-s[i].zh)*(z2-s[i].zh)+(x2-s[i].xh)*(x2-s[i].xh);
		s[i].rho = sqrt(s[i].dd2)/tan(s[i].dtheta/2.);
		if(fabs(s[i].rho) < 500 || fabs(s[i].rho) > 100000.){ s[i].x1 = -9999.; continue; }
		s[i].zo = (k1*(s[i].ki*(s[i].x1 - x2) + z1) - s[i].ki*z2) / (k1 - s[i].ki);
		s[i].xo = (-s[i].ki*s[i].x1 + k1*x2 - z1 + z2) / (k1 - s[i].ki);
	}




