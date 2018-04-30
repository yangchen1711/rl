//
// Copyright (c) 2009, Markus Rickert
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef RL_MATH_MATRIXBASEADDONS_H
#define RL_MATH_MATRIXBASEADDONS_H

#ifdef DOXYGEN_SHOULD_PARSE_THIS
namespace Eigen { template<typename Derived> class MatrixBase {
#endif

template<typename OtherDerived>
Matrix<Scalar, 2, 1>
static CartesianFromPolar(const MatrixBase<OtherDerived>& polar)
{
	using ::std::cos;
	using ::std::sin;
	
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 2)
	
	eigen_assert(2 == polar.size());
	
	return Matrix<Scalar, 2, 1>(
		polar.x() * cos(polar.y()),
		polar.x() * sin(polar.y())
	);
}

template<typename OtherDerived>
Matrix<Scalar, 3, 1>
static CartesianFromSpherical(const MatrixBase<OtherDerived>& spherical)
{
	using ::std::cos;
	using ::std::sin;
	
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 3)
	
	eigen_assert(3 == spherical.size());
	
	Scalar sin_y = sin(spherical.y());
	
	return Matrix<Scalar, 3, 1>(
		spherical.x() * sin_y * cos(spherical.z()),
		spherical.x() * sin_y * sin(spherical.z()),
		spherical.x() * cos(spherical.y())
	);
}

template<typename OtherDerived>
Matrix<Scalar, 2, 1>
static PolarFromCartesian(const MatrixBase<OtherDerived>& cartesian)
{
	using ::std::atan2;
	using ::std::pow;
	using ::std::sqrt;
	
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 2)
	
	eigen_assert(2 == cartesian.size());
	
	return Matrix<Scalar, 2, 1>(
		sqrt(pow(cartesian.x(), 2) + pow(cartesian.y(), 2)),
		atan2(cartesian.y(), cartesian.x())
	);
}

Matrix<Scalar, 2, 1>
static RandomOnCircle()
{
	return RandomOnCircle(internal::random<Scalar>(0, 1));
}

/**
 * Generate random point on unit circle.
 * 
 * http://mathworld.wolfram.com/CirclePointPicking.html
 */
Matrix<Scalar, 2, 1>
static RandomOnCircle(const Scalar& rand)
{
	using ::std::cos;
	using ::std::sin;
	
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 2)
	
	eigen_assert(rand >= Scalar(0));
	eigen_assert(rand <= Scalar(1));
	
	Scalar theta = Scalar(2 * M_PI) * rand;
	
	return Matrix<Scalar, 2, 1>(
		cos(theta),
		sin(theta)
	);
}

Matrix<Scalar, 3, 1>
static RandomOnSphere()
{
	return RandomOnSphere(
		Vector2(
			internal::random<Scalar>(0, 1),
			internal::random<Scalar>(0, 1)
		)
	);
}

/**
 * Generate random point on unit sphere.
 * 
 * http://mathworld.wolfram.com/SpherePointPicking.html
 */
template<typename OtherDerived>
Matrix<Scalar, 3, 1>
static RandomOnSphere(const DenseBase<OtherDerived>& rand)
{
	using ::std::cos;
	using ::std::pow;
	using ::std::sin;
	using ::std::sqrt;
	
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 3)
	
	eigen_assert(2 == rand.size());
	eigen_assert(rand(0) >= Scalar(0));
	eigen_assert(rand(0) <= Scalar(1));
	eigen_assert(rand(1) >= Scalar(0));
	eigen_assert(rand(1) <= Scalar(1));
	
	Scalar theta = Scalar(2 * M_PI) * rand(0);
	Scalar z = Scalar(2) * rand(1) - Scalar(1);
	Scalar sqrt_1_z_2 = sqrt(1 - pow(z, 2));
	
	return Matrix<Scalar, 3, 1>(
		sqrt_1_z_2 * cos(theta),
		sqrt_1_z_2 * sin(theta),
		z
	);
}

template<typename OtherDerived>
Matrix<Scalar, 3, 1>
static SphericalFromCartesian(const MatrixBase<OtherDerived>& cartesian)
{
	using ::std::atan2;
	using ::std::pow;
	using ::std::sqrt;
	
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 3)
	
	eigen_assert(3 == cartesian.size());
	
	Scalar x_2 = pow(cartesian.x(), 2);
	Scalar y_2 = pow(cartesian.y(), 2);
	Scalar z_2 = pow(cartesian.z(), 2);
	
	return Matrix<Scalar, 3, 1>(
		sqrt(x_2 + y_2 + z_2),
		atan2(sqrt(x_2 + y_2), cartesian.z()),
		atan2(cartesian.y(), cartesian.x())
	);
}

Matrix<Scalar, 3, 1>
cross3() const
{
	EIGEN_STATIC_ASSERT_MATRIX_SPECIFIC_SIZE(Derived, 3, 3)
	Matrix<Scalar, 3, 1> res;
	res.x() = (this->derived()(2, 1) - this->derived()(1, 2)) / Scalar(2);
	res.y() = (this->derived()(0, 2) - this->derived()(2, 0)) / Scalar(2);
	res.z() = (this->derived()(1, 0) - this->derived()(0, 1)) / Scalar(2);
	return res;
}

Matrix<Scalar, 3, 3>
cross33() const
{
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 3)
	Matrix<Scalar, 3, 3> res;
	res(0, 0) = 0;
	res(0, 1) = -this->derived().z();
	res(0, 2) = this->derived().y();
	res(1, 0) = this->derived().z();
	res(1, 1) = 0;
	res(1, 2) = -this->derived().x();
	res(2, 0) = -this->derived().y();
	res(2, 1) = this->derived().x();
	res(2, 2) = 0;
	return res;
}

Derived&
setRandomOnCircle()
{
	return this->derived() = RandomOnCircle();
}

template<typename OtherDerived>
Derived&
setRandomOnCircle(const Scalar& rand)
{
	return this->derived() = RandomOnCircle(rand);
}

Derived&
setRandomOnSphere()
{
	return this->derived() = RandomOnSphere();
}

template<typename OtherDerived>
Derived&
setRandomOnSphere(const DenseBase<OtherDerived>& rand)
{
	return this->derived() = RandomOnSphere(rand);
}

Matrix<Scalar, 6, 1>
voigt6() const
{
	EIGEN_STATIC_ASSERT_MATRIX_SPECIFIC_SIZE(Derived, 3, 3)
	Matrix<Scalar, 6, 1> res;
	res(0) = this->derived()(0, 0);
	res(1) = this->derived()(1, 1);
	res(2) = this->derived()(2, 2);
	res(3) = (this->derived()(1, 2) + this->derived()(2, 1)) / Scalar(2);
	res(4) = (this->derived()(0, 2) + this->derived()(2, 0)) / Scalar(2);
	res(5) = (this->derived()(0, 1) + this->derived()(1, 0)) / Scalar(2);
	return res;
}

Matrix<Scalar, 3, 3>
voigt33() const
{
	EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived, 6)
	Matrix<Scalar, 3, 3> res;
	res(0, 0) = this->derived()(0);
	res(0, 1) = this->derived()(5);
	res(0, 2) = this->derived()(4);
	res(1, 0) = this->derived()(5);
	res(1, 1) = this->derived()(1);
	res(1, 2) = this->derived()(3);
	res(2, 0) = this->derived()(4);
	res(2, 1) = this->derived()(3);
	res(2, 2) = this->derived()(2);
	return res;
}

#ifdef DOXYGEN_SHOULD_PARSE_THIS
} }
#endif

#endif // RL_MATH_MATRIXBASEADDONS_H
