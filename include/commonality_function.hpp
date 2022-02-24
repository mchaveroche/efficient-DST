#ifndef EFFICIENT_DST_COMMONALITY_FUNCTION_HPP
#define EFFICIENT_DST_COMMONALITY_FUNCTION_HPP

#include <mass_function.hpp>
#include <conjunctive_decomposition.hpp>
#include <zeta_transform.hpp>

namespace efficient_DST{

	template <size_t N, typename T = float>
	class commonality_function : public zeta_transform<up_inclusion<N, T>, N, T> {
	public:

		commonality_function(
			const mass_function<N, T>& m
		) : zeta_transform<up_inclusion<N, T>, N, T>(m.get_sample_space(), m.get_definition(), m.get_default_value(), operation_type_t::addition)
		{}

		commonality_function(
			const weight_function<N, T>& w
		) : zeta_transform<up_inclusion<N, T>, N, T>(w.get_sample_space(), w.get_definition(), w.get_default_value(), operation_type_t::multiplication)
		{}

		commonality_function(
			const conjunctive_decomposition<N, T>& w
		) : zeta_transform<up_inclusion<N, T>, N, T>(w.get_sample_space(), w.get_definition(), w.get_default_value(), operation_type_t::multiplication)
		{
			this->normalize(w.normalizing_set, w.normalizing_value);
		}

		commonality_function(
			const commonality_function<N, T>& q
		) : zeta_transform<up_inclusion<N, T>, N, T>(q)
		{}

//		commonality_function(
//			const powerset_btree<N, T>& focal_points_values,
//			const scheme_type_t& scheme_type,
//			const std::vector<subset >& iota_sequence,
//			const T& neutral_value
//		) : zeta_transform<up_inclusion<N, T>, N, T>(
//				focal_points_values,
//				scheme_type,
//				iota_sequence,
//				neutral_value
//			)
//		{}


		template <class fusion_rule>
		commonality_function<N, T> fuse_with(const commonality_function<N, T>& q2) const {
			const fusion_rule fusion;
			return fusion(*this, q2);
		}
	};

} // namespace efficient_DST

#endif // EFFICIENT_DST_COMMONALITY_FUNCTION_HPP
