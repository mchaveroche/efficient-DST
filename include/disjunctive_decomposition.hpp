#ifndef EFFICIENT_DST_DISJUNCTIVE_DECOMPOSITION_HPP
#define EFFICIENT_DST_DISJUNCTIVE_DECOMPOSITION_HPP

#include <decomposition.hpp>
#include <zeta_transform.hpp>

namespace efficient_DST{

	template <size_t N, typename T = float, bool adaptive_uncertainty = true>
	class disjunctive_decomposition : public decomposition<down_inclusion<N, T>, N, T, adaptive_uncertainty> {
	public:
		using typename powerset_function<N, T>::subset;
		using powerset_function<N, T>::emptyset;
		using powerset_function<N, T>::fullset;

		disjunctive_decomposition(const weight_function<N, T>& w) : decomposition<down_inclusion<N, T>, N, T, adaptive_uncertainty>(w)
		{}

		disjunctive_decomposition(const sample_space<N>& outcomes) : decomposition<down_inclusion<N, T>, N, T, adaptive_uncertainty>(outcomes)
		{}

		disjunctive_decomposition(
			const zeta_transform<down_inclusion<N, T>, N, T>& b
		) : decomposition<down_inclusion<N, T>, N, T, adaptive_uncertainty>(b)
		{}

		template <class fusion_rule>
		disjunctive_decomposition<N, T, adaptive_uncertainty> fuse_with(const disjunctive_decomposition<N, T, adaptive_uncertainty>& v2) const {
			const fusion_rule fusion;
			return fusion(*this, v2);
		}
	};

} // namespace efficient_DST

#endif // EFFICIENT_DST_DISJUNCTIVE_DECOMPOSITION_HPP
