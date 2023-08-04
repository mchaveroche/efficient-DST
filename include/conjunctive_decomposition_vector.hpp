/*
 * Copyright (C) 2019-2023  Maxime Chaveroche (maxime.chaveroche@gmail.com)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the CeCILL License, either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * CeCILL License for more details.
 * 
 * You should have received a copy of the CeCILL License
 * along with this program. If not, see <http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.html>.
 */

#ifndef EFFICIENT_DST_CONJUNCTIVE_DECOMPOSITION_VECTOR_HPP
#define EFFICIENT_DST_CONJUNCTIVE_DECOMPOSITION_VECTOR_HPP

#include <decomposition_vector.hpp>
#include <zeta_transform_vector.hpp>

namespace efficient_DST{

	template <size_t N, typename T = float>
	class conjunctive_decomposition_vector : public decomposition_vector<up_inclusion<N, T>, N, T> {
	public:

		conjunctive_decomposition_vector(
			const weight_vector<N, T>& w,
			const bool& adaptive_uncertainty = true
		) : decomposition_vector<up_inclusion<N, T>, N, T>(w, adaptive_uncertainty)
		{}

		conjunctive_decomposition_vector(
			const sample_space<N>& outcomes,
			const bool& adaptive_uncertainty = true
		) : decomposition_vector<up_inclusion<N, T>, N, T>(outcomes, adaptive_uncertainty)
		{}

		conjunctive_decomposition_vector(
			const zeta_transform_vector<up_inclusion<N, T>, N, T>& q,
			const bool& adaptive_uncertainty = true
		) : decomposition_vector<up_inclusion<N, T>, N, T>(q, q.reduced_core(q.get_definition()), 0, adaptive_uncertainty)
		{}


		template <class fusion_rule>
		conjunctive_decomposition_vector<N, T> fuse_with(const conjunctive_decomposition_vector<N, T>& w2) const {
			const fusion_rule fusion;
			return fusion(*this, w2);
		}
	};

} // namespace efficient_DST

#endif // EFFICIENT_DST_CONJUNCTIVE_DECOMPOSITION_VECTOR_HPP
