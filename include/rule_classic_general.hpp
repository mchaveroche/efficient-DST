#ifndef EFFICIENT_DST_RULE_CLASSIC_GENERAL_HPP
#define EFFICIENT_DST_RULE_CLASSIC_GENERAL_HPP

#include <sample_space.hpp>
#include <commonality_function.hpp>
#include <conjunctive_weight.hpp>
#include <mass.hpp>

namespace efficient_DST{

	template <class inclusion, size_t N, typename T = float>
	class rule_classic_general {
	public:
		typedef typename sample_space<N>::subset subset;

		std::string to_string() const {
			return "General classic rule";
		}

		virtual ~rule_classic_general(){}

		mass<N, T> operator()(
			const mass<N, T>& m1,
			const mass<N, T>& m2
		) const {
			const std::vector<set_N_value<N, T>* >& focal_sets_1 = m1.get_definition().elements();
			const std::vector<set_N_value<N, T>* >& focal_sets_2 = m2.get_definition().elements();
			mass<N, T> m12(m1.get_sample_space());
			powerset_btree<N, T>& focal_sets_12 = m12.definition_();

			for (size_t i1 = 0; i1 < focal_sets_1.size(); ++i1){
				for (size_t i2 = 0; i2 < focal_sets_2.size(); ++i2){
					const subset& set = inclusion::set_operation(focal_sets_1[i1]->set, focal_sets_2[i2]->set);
					set_N_value<N, T>* node = focal_sets_12[set];
					if (node){
						node->value += focal_sets_1[i1]->value * focal_sets_2[i2]->value;
					}else{
						focal_sets_12.insert(set, focal_sets_1[i1]->value * focal_sets_2[i2]->value);
					}
				}
			}
//			mass<N, T> m12(m1.get_sample_space(), focal_sets_12);
			m12.remove_negligible_values();
			m12.normalize();
			return m12;
		}

	protected:

		static T multiply(const T& val1, const T& val2){
			return val1 * val2;
		}

//		powerset_btree<T, N> weight_fusion(const powerset_btree<T, N>& w1_definition, const powerset_btree<T, N>& w2_definition) const {
//			powerset_btree<T, N> w12_definition(w1_definition.get_FOD(), w1_definition.get_block_size());
//			w12_definition.fill_with_union_of_powersets(w1_definition, w2_definition, multiply, 1);
//			decomposition_weight<T, N>::remove_negligible_values(w12_definition);
//			decomposition_weight<T, N>::normalize(w12_definition);
//			return w12_definition;
//		}
	};

} // namespace efficient_DST

#endif // EFFICIENT_DST_RULE_CLASSIC_GENERAL_HPP
