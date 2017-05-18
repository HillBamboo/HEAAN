#include "EvaluatorUtils.h"

CZZ EvaluatorUtils::evaluateVal(const double& xr, const double& xi, const long& logp) {
	if(logp < 31) {
		long p = 1 << logp;
		ZZ pxexpr = to_ZZ(xr * p);
		ZZ pxexpi = to_ZZ(xi * p);
		return CZZ(pxexpr, pxexpi);
	} else {
		long tmp = (1 << 30);
		ZZ pxexpr = to_ZZ(xr * tmp) << (logp - 30);
		ZZ pxexpi = to_ZZ(xi * tmp) << (logp - 30);
		return CZZ(pxexpr, pxexpi);
	}
}

CZZ EvaluatorUtils::evaluateRandomVal(const long& logp) {
	double mr = (double)arc4random() / RAND_MAX;
	double mi = (double)arc4random() / RAND_MAX;
	return evaluateVal(mr, mi, logp);
}

CZZ EvaluatorUtils::evaluateRandomCircleVal(const long& logp) {
	double angle = (double)arc4random() / RAND_MAX;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);
	return evaluateVal(mr, mi, logp);
}

void EvaluatorUtils::evaluateRandomVals(vector<CZZ>& res, const long& size, const long& logp) {
	res.reserve(size);
	for (long i = 0; i < size; ++i) {
		CZZ m = EvaluatorUtils::evaluateRandomVal(logp);
		res.push_back(m);
	}
}

CZZ EvaluatorUtils::evaluatePow(const double& xr, const double& xi, const long& degree, const long& logp) {
	long logDegree = log2(degree);
	long po2Degree = 1 << logDegree;
	CZZ res = evaluatePow2(xr, xi, logDegree, logp);
	long remDegree = degree - po2Degree;
	if(remDegree > 0) {
		CZZ tmp = evaluatePow(xr, xi, remDegree, logp);
		res *= tmp;
		res >>= logp;
	}
	return res;
}

CZZ EvaluatorUtils::evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	CZZ res = evaluateVal(xr, xi, logp);
	for (int i = 0; i < logDegree; ++i) {
		res = (res * res) >> logp;
	}
	return res;
}

vector<CZZ> EvaluatorUtils::evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp) {
	vector<CZZ> res;
	CZZ m = evaluateVal(xr, xi, logp);
	res.push_back(m);
	for (int i = 0; i < degree - 1; ++i) {
		CZZ pow = (res[i] * m) >> logp;
		res.push_back(pow);
	}
	return res;
}

vector<CZZ> EvaluatorUtils::evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp) {
	vector<CZZ> res;
	CZZ m = evaluateVal(xr, xi, logp);
	res.push_back(m);
	for (int i = 0; i < logDegree; ++i) {
		CZZ pow2 = (res[i] * res[i]) >> logp;
		res.push_back(pow2);
	}
	return res;
}

CZZ EvaluatorUtils::evaluateInverse(const double& xr, const double& xi, const long& logp) {
	double xinvr = xr / (xr * xr + xi * xi);
	double xinvi = -xi / (xr * xr + xi * xi);

	return evaluateVal(xinvr, xinvi, logp);
}

CZZ EvaluatorUtils::evaluateExponent(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	return evaluateVal(xexpr, xexpi, logp);
}

CZZ EvaluatorUtils::evaluateSigmoid(const double& xr, const double& xi, const long& logp) {
	double xrexp = exp(xr);
	double xexpr = xrexp * cos(xi);
	double xexpi = xrexp * sin(xi);

	double xsigmoidr = (xexpr * (xexpr + 1) + (xexpi * xexpi)) / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));
	double xsigmoidi = xexpi / ((xexpr + 1) * (xexpr + 1) + (xexpi * xexpi));

	return evaluateVal(xsigmoidr, xsigmoidi, logp);
}

void EvaluatorUtils::evaluateRandomCircleValsAndPows(vector<CZZ>& vals, vector<CZZ>& fvals, const long& size, const long& degree, const long& logp) {
	for (long i = 0; i < size; ++i) {
		double angle = (double)arc4random() / RAND_MAX;
		double mr = cos(angle * 2 * Pi);
		double mi = sin(angle * 2 * Pi);

		CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
		CZZ mpow = EvaluatorUtils::evaluatePow(mr, mi, degree, logp);

		vals.push_back(m);
		fvals.push_back(mpow);
	}
}

void EvaluatorUtils::evaluateRandomCircleValsAndPows2(vector<CZZ>& vals, vector<CZZ>& fvals, const long& size, const long& logDegree, const long& logp) {
	for (long i = 0; i < size; ++i) {
		double angle = (double)arc4random() / RAND_MAX;
		double mr = cos(angle * 2 * Pi);
		double mi = sin(angle * 2 * Pi);

		CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
		CZZ mpow = EvaluatorUtils::evaluatePow2(mr, mi, logDegree, logp);

		vals.push_back(m);
		fvals.push_back(mpow);
	}
}

void EvaluatorUtils::evaluateRandomCircleValsAndProduct(vector<vector<CZZ>>& vals, vector<CZZ>& fvals, const long& slots, const long& size, const long& logp) {
	vals.reserve(size);
	for (long i = 0; i < size; ++i) {
		vector<CZZ> tmp;
		for (long j = 0; j < slots; ++j) {
			CZZ m = evaluateRandomCircleVal(logp);
			tmp.push_back(m);
		}
		vals.push_back(tmp);
	}

	for (long j = 0; j < slots; ++j) {
		CZZ prod = vals[0][j];
		for (long i = 1; i < size; ++i) {
			prod *= vals[i][j];
			prod >>= logp;
		}
		fvals.push_back(prod);
	}
}

void EvaluatorUtils::evaluateRandomCircleBarValsAndInverses(vector<CZZ>& vals, vector<CZZ>& fvals, const long& size, const long& logp) {
	vals.reserve(size);
	fvals.reserve(size);
	for (long i = 0; i < size; ++i) {
		double angle = (double)arc4random() / RAND_MAX / 50;

		double mr = cos(angle * 2 * Pi);
		double mi = sin(angle * 2 * Pi);

		CZZ mbar = EvaluatorUtils::evaluateVal(1 - mr, -mi, logp);
		CZZ minv = EvaluatorUtils::evaluateInverse(mr, mi, logp);

		vals.push_back(mbar);
		fvals.push_back(minv);
	}
}

void EvaluatorUtils::evaluateRandomValsAndExponents(vector<CZZ>& vals, vector<CZZ>& fvals, const long& size, const long& logp) {
	vals.reserve(size);
	fvals.reserve(size);
	for (long i = 0; i < size; ++i) {
		double mr = (double)arc4random() / RAND_MAX;
		double mi = (double)arc4random() / RAND_MAX;
		CZZ m = evaluateVal(mr, mi, logp);
		CZZ fm = evaluateExponent(mr, mi, logp);
		vals.push_back(m);
		fvals.push_back(fm);
	}
}

void EvaluatorUtils::evaluateRandomValsAndSigmoids(vector<CZZ>& vals, vector<CZZ>& fvals, const long& size, const long& logp) {
	vals.reserve(size);
	fvals.reserve(size);
	for (long i = 0; i < size; ++i) {
		double mr = (double)arc4random() / RAND_MAX;
		double mi = (double)arc4random() / RAND_MAX;
		CZZ m = evaluateVal(mr, mi, logp);
		CZZ fm = evaluateSigmoid(mr, mi, logp);
		vals.push_back(m);
		fvals.push_back(fm);
	}
}

void EvaluatorUtils::leftShift(vector<CZZ>& vals, const long& logp) {
	for (long i = 0; i < vals.size(); ++i) {
		vals[i] <<= logp;
	}
}
