#ifndef EFFICIENT_DST_ZETA_TRANSFORM_HPP
#define EFFICIENT_DST_ZETA_TRANSFORM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <powerset_function.hpp>
#include <mobius_inversion.hpp>


namespace efficient_DST{

	enum class operation_type_t: bool { addition, multiplication };

	template <class inclusion, size_t N, typename T = float>
	class zeta_transform : public powerset_function<N, T> {
	protected:
		using typename powerset_function<N, T>::subset;
		using powerset_function<N, T>::operator[];

		scheme_type_t scheme_type;
		std::vector<subset > iota_sequence;
		std::map<size_t, powerset_btree<N, set_N_value<N, T>* > > definition_by_cardinality;

	public:

		zeta_transform(const zeta_transform<inclusion, N, T>& z) :
			powerset_function<N, T>(z.outcomes, z.definition, z.default_value),
			scheme_type(z.scheme_type),
			iota_sequence(z.iota_sequence),
			definition_by_cardinality(z.definition_by_cardinality)
		{}

//		zeta_transform(
//			const sample_space<N>& outcomes,
//			const powerset_btree<N, T>& definition,
//			const scheme_type_t& scheme_type,
//			const std::vector<subset >& iota_sequence,
//			const T& neutral_value
//		) :
//			powerset_function<N, T>(outcomes, definition),
//			scheme_type(scheme_type),
//			iota_sequence(iota_sequence),
//			neutral_value(neutral_value)
//		{
//			this->set_definition_by_cardinality();
//		}

		/*
		 * Constructor when you have a Möbius transform such as the mass or conjunctive/disjunctive weight function.
		 * - support is supposed to contain all focal sets and their image.
		 * - order_relation is the order relation of this zeta transform (e.g. commonality->superset or implicability->subset).
		 * - transform_operation is the operation of the zeta transform (e.g. in DST, we usually use the addition on the mass function
		 * and the multiplication on the conjunctive/disjunctive weight function).
		 */
		zeta_transform(
			const sample_space<N>& outcomes,
			const powerset_btree<N, T>& support,
			const T& default_value,
			operation_type_t operation_type
		) :
			powerset_function<N, T>(outcomes, N * support.size(), default_value),
			scheme_type(scheme_type_t::direct)
		{
			if (operation_type == operation_type_t::addition){
				this->compute<addition<T>>(support);
			} else {
				this->compute<multiplication<T>>(support);
			}
			this->set_definition_by_cardinality();
		}

//		template<class operation_type>
//		powerset_btree<N, T> inversion() const {
		powerset_btree<N, T> inversion(const operation_type_t& operation_type) const {
			powerset_btree<N, T> mobius_transform_definition(this->definition);
			if (operation_type == operation_type_t::addition){
				efficient_mobius_inversion<inclusion, mobius_tranformation<inclusion, addition<T>, N, T>, N, T >::execute(
						mobius_transform_definition,
						this->iota_sequence,
						this->scheme_type
				);
			} else {
				efficient_mobius_inversion<inclusion, mobius_tranformation<inclusion, multiplication<T>, N, T>, N, T >::execute(
						mobius_transform_definition,
						this->iota_sequence,
						this->scheme_type
				);
			}
			return mobius_transform_definition;
		}

		T operator[](const subset& set) const {
			set_N_value<N, T>* set_value = this->definition[set];
			if(set_value){
				return set_value->value;
			}
			return this->find_non_focal_point_image(set);
		}

	protected:

		T find_non_focal_point_image(const subset& set) const {
			set_N_value<N, set_N_value<N, T>* >* A = nullptr;
			const size_t& card = set.count();

			for (const auto& c_focal_points : this->definition_by_cardinality) {
				if(inclusion::naturally_ranked(c_focal_points.first, card)){
					const powerset_btree<N, set_N_value<N, T>* >& p = c_focal_points.second;
					const std::vector<set_N_value<N, set_N_value<N, T>* >* >& related_elements = inclusion::addresses_related_to(p, set);

					if(related_elements.size() > 0){
						A = related_elements[0];
						break;
					}
				}
			}
			if (A){
				return A->value->value;
			}else{
				return this->default_value;
			}
		}


		void set_definition_by_cardinality(){
			const auto& definition_card_map = inclusion::card_mapping_dual(this->definition);
			// create a powerset_btree in this->definition_by_cardinality for each vector of elements in definition_card_map
			for (const auto& c_focal_points : definition_card_map) {
				this->definition_by_cardinality.emplace(
					std::piecewise_construct,
					std::make_tuple(c_focal_points.first),
					std::make_tuple(this->definition.get_block_size())
				);
				powerset_btree<N, set_N_value<N, T>* >& p_c = this->definition_by_cardinality[c_focal_points.first];
				const std::vector<set_N_value<N, T>* >& elements = c_focal_points.second;
				for(size_t i = 0; i < elements.size(); ++i){
					p_c.insert(elements[i]->set, elements[i]);
				}
			}
		}

		template<class operation_type>
		void compute(const powerset_btree<N, T>& support){
			if (this->default_value != operation_type::neutral_value()){
				std::cerr << "Ill-defined support: the default value of your compact definition must match the neutral value for the operator you chose.\n";
				exit(1);
			}
			this->scheme_type = efficient_mobius_inversion<
				inclusion, zeta_tranformation<inclusion, operation_type, N, T>, N, T
			>::autoset_and_build(
					support,
					this->definition,
					this->iota_sequence
			);

			efficient_mobius_inversion<
				inclusion, zeta_tranformation<inclusion, operation_type, N, T>, N, T
			>::execute(
					this->definition,
					this->iota_sequence,
					this->scheme_type
			);
//			this->default_value = operation_type::neutral_value();
		}
	};
}	// namespace efficient_DST

#endif // EFFICIENT_DST_ZETA_TRANSFORM_HPP
