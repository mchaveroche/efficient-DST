#ifndef EFFICIENT_DST_MOBIUS_INVERSION_TEMPLATE_HPP
#define EFFICIENT_DST_MOBIUS_INVERSION_TEMPLATE_HPP

#include "macros.hpp"
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <vector>
#include <unordered_set>
//#include <iostream>
//#include <iomanip>
//#include <utility>
//#include <functional>

#include <powerset_btree.hpp>


namespace efficient_DST{

//	template<typename T, size_t N>
//	struct mobius_inversion_basic_operation {
//
//		static inline size_t subgraph_index_mobius(const size_t& n, const size_t& i){
//			return i;
//		}
//
//		static inline size_t subgraph_index_zeta(const size_t& n, const size_t& i){
//			return n - i;
//		}
//
//		static inline size_t subgraph_dual_index_mobius(const size_t& n, const size_t& i){
//			return n - i;
//		}
//
//		static inline size_t subgraph_dual_index_zeta(const size_t& n, const size_t& i){
//			return i;
//		}
//
//		static inline void value_inplace_operation_zeta_additive(T& a, const T& b){
//			a += b;
//		}
//
//		static inline void value_inplace_operation_mobius_additive(T& a, const T& b){
//			a -= b;
//		}
//
//		static inline void value_inplace_operation_zeta_multiplicative(T& a, const T& b){
//			a *= b;
//		}
//
//		static inline void value_inplace_operation_mobius_multiplicative(T& a, const T& b){
//			a /= b;
//		}
//
//		static inline size_t target_index_offset_FMT_down_inclusion(){
//			return 0;
//		}
//
//		static inline size_t target_index_offset_FMT_up_inclusion(){
//			return 1;
//		}
//
//		static inline size_t source_index_offset_FMT_down_inclusion(){
//			return 1;
//		}
//
//		static inline size_t source_index_offset_FMT_up_inclusion(){
//			return 0;
//		}
//
//		static inline std::bitset<N> set_operation_down_inclusion(const std::bitset<N>& a, const std::bitset<N>& b){
//			return a | b;
//		}
//
//		static inline std::bitset<N> set_operation_up_inclusion(const std::bitset<N>& a, const std::bitset<N>& b){
//			return a & b;
//		}
//
//		static inline std::bitset<N> set_dual_operation_down_inclusion(const std::bitset<N>& a, const std::bitset<N>& b){
//			return a & b;
//		}
//
//		static inline std::bitset<N> set_dual_operation_up_inclusion(const std::bitset<N>& a, const std::bitset<N>& b){
//			return a | b;
//		}
//
//		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::less<size_t> > card_mapping_down_inclusion(const powerset_btree<T, N>& tree){
//			return tree.elements_by_ascending_cardinality();
//		}
//
//		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::greater<size_t> > card_mapping_up_inclusion(const powerset_btree<T, N>& tree){
//			return tree.elements_by_descending_cardinality();
//		}
//
//		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::greater<size_t> > card_mapping_dual_down_inclusion(const powerset_btree<T, N>& tree){
//			return tree.elements_by_descending_cardinality();
//		}
//
//		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::less<size_t> > card_mapping_dual_up_inclusion(const powerset_btree<T, N>& tree){
//			return tree.elements_by_ascending_cardinality();
//		}
//
//		static inline std::vector<set_N_value<T, N>* > elements_down_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
//			return tree.subsets_of(set);
//		}
//
//		static inline std::vector<set_N_value<T, N>* > elements_up_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
//			return tree.supersets_of(set);
//		}
//
//		static inline std::vector<set_N_value<T, N>* > elements_dually_down_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
//			return tree.supersets_of(set);
//		}
//
//		static inline std::vector<set_N_value<T, N>* > elements_dually_up_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
//			return tree.subsets_of(set);
//		}
//	};

	template<typename T, size_t N>
	struct iota_elements {

		static inline std::vector<std::bitset<N> > join_irreducible(
			const powerset_btree<T, N>& support
		){
			// join-irreducible elements of the lattice support
			std::vector<std::bitset<N> > sequence;
			std::unordered_set<std::bitset<N> > manifest;
			std::map<size_t, std::vector<std::bitset<N> >, std::less<size_t> > iota_elements_card_map;
			std::bitset<N> singleton = 1;
			for (size_t i = 0; i < N; ++i) {
				const std::vector<set_N_value<T, N>* >& support_supersets = support.supersets_of(singleton);

				if (support_supersets.size() > 0) {
					std::bitset<N> iota_element((const std::bitset<N>&) support_supersets[0]->set);

					for (size_t ii = 1; ii < support_supersets.size(); ++ii) {
						iota_element &= support_supersets[ii]->set;
						if (iota_element == singleton) {
							break;
						}
					}
					bool insertion = manifest.emplace(iota_element).second;
					if (insertion){
						size_t cardinality = iota_element.count();
						if (iota_elements_card_map.find(cardinality) == iota_elements_card_map.end()){
							iota_elements_card_map.emplace(cardinality, std::vector<std::bitset<N> >());
						}
						iota_elements_card_map[cardinality].emplace_back(iota_element);
					}
				}
				singleton <<= 1;
			}
			sequence.reserve(manifest.size());

			for (const auto& c_iota_elements : iota_elements_card_map) {
				const std::vector<std::bitset<N> >& elements = c_iota_elements.second;
				for (size_t i = 0; i < elements.size(); ++i) {
					sequence.emplace_back(elements[i]);
				}
			}
			return sequence;
		}

		static inline std::vector<std::bitset<N> > meet_irreducible(
			const powerset_btree<T, N>& support
		){
			// meet-irreducible elements of the lattice support
			std::vector<std::bitset<N> > sequence;
			std::unordered_set<std::bitset<N> > manifest;
			std::map<size_t, std::vector<std::bitset<N> >, std::greater<size_t> > iota_elements_card_map;
			std::bitset<N> singleton = 1;
			std::bitset<N> singleton_dual = ~singleton;
			for (size_t i = 0; i < N; ++i) {
				const std::vector<set_N_value<T, N>* >& support_subsets = support.subsets_of(singleton_dual);

				if (support_subsets.size() > 0) {
					std::bitset<N> iota_element((const std::bitset<N>&) support_subsets[0]->set);
					for (size_t ii = 1; ii < support_subsets.size(); ++ii) {
						iota_element |= support_subsets[ii]->set;
						if (iota_element == singleton_dual) {
							break;
						}
					}
					bool insertion = manifest.emplace(iota_element).second;
					if (insertion){
						size_t cardinality = iota_element.count();
						if (iota_elements_card_map.find(cardinality) == iota_elements_card_map.end()){
							iota_elements_card_map.emplace(cardinality, std::vector<std::bitset<N> >());
						}
						iota_elements_card_map[cardinality].emplace_back(iota_element);
					}
				}
				singleton <<= 1;
				singleton_dual = ~singleton;
			}
			sequence.reserve(manifest.size());

			for (const auto& c_iota_elements : iota_elements_card_map) {
				const std::vector<std::bitset<N> >& elements = c_iota_elements.second;
				for (size_t i = 0; i < elements.size(); ++i) {
					sequence.emplace_back(elements[i]);
				}
			}
			return sequence;
		}
	};

	template<typename T, size_t N>
	struct up_inclusion {

		static inline std::vector<std::bitset<N> > compute_iota_elements(
			const powerset_btree<T, N>& support
		){
			return iota_elements<T, N>::meet_irreducible(support);
		}

		static inline std::vector<std::bitset<N> > compute_iota_elements_dual(
			const powerset_btree<T, N>& support
		){
			return iota_elements<T, N>::join_irreducible(support);
		}

		static inline size_t target_index_offset_FMT(){
			return 1;
		}

		static inline size_t source_index_offset_FMT(){
			return 0;
		}

		static inline std::bitset<N> set_operation(const std::bitset<N>& a, const std::bitset<N>& b){
			return a & b;
		}

		static inline std::bitset<N> set_dual_operation(const std::bitset<N>& a, const std::bitset<N>& b){
			return a | b;
		}

		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::greater<size_t> > card_mapping(const powerset_btree<T, N>& tree){
			return tree.elements_by_descending_cardinality();
		}

		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::less<size_t> > card_mapping_dual(const powerset_btree<T, N>& tree){
			return tree.elements_by_ascending_cardinality();
		}

		static inline std::vector<set_N_value<T, N>* > elements_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
			return tree.supersets_of(set);
		}

		static inline std::vector<set_N_value<T, N>* > elements_dually_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
			return tree.subsets_of(set);
		}

		static inline std::bitset<N> absorbing_set_for_operation(){
			return 0;
		}

		static inline std::bitset<N> absorbing_set_for_dual_operation(){
			return ~absorbing_set_for_operation();
		}
	};

	template<typename T, size_t N>
	struct down_inclusion {

		static inline std::vector<std::bitset<N> > compute_iota_elements(
			const powerset_btree<T, N>& support
		){
			return iota_elements<T, N>::join_irreducible(support);
		}

		static inline std::vector<std::bitset<N> > compute_iota_elements_dual(
			const powerset_btree<T, N>& support
		){
			return iota_elements<T, N>::meet_irreducible(support);
		}

		static inline size_t target_index_offset_FMT(){
			return 0;
		}

		static inline size_t source_index_offset_FMT(){
			return 1;
		}

		static inline std::bitset<N> set_operation(const std::bitset<N>& a, const std::bitset<N>& b){
			return a | b;
		}

		static inline std::bitset<N> set_dual_operation(const std::bitset<N>& a, const std::bitset<N>& b){
			return a & b;
		}

		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::greater<size_t> > card_mapping(const powerset_btree<T, N>& tree){
			return tree.elements_by_ascending_cardinality();
		}

		static inline std::map<size_t, std::vector<set_N_value<T, N>* >, std::less<size_t> > card_mapping_dual(const powerset_btree<T, N>& tree){
			return tree.elements_by_descending_cardinality();
		}

		static inline std::vector<set_N_value<T, N>* > elements_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
			return tree.subsets_of(set);
		}

		static inline std::vector<set_N_value<T, N>* > elements_dually_related_to(const powerset_btree<T, N>& tree, const std::bitset<N>& set){
			return tree.supersets_of(set);
		}

		static inline std::bitset<N> absorbing_set_for_operation(){
			return ~absorbing_set_for_dual_operation();
		}

		static inline std::bitset<N> absorbing_set_for_dual_operation(){
			return 0;
		}
	};

	template<typename T>
	struct zeta_additive_operation {

		static const T neutral_value = 0;

		void operator()(T& a, const T& b) const {
			a += b;
		}
	};

	template<typename T>
	struct mobius_additive_operation {

		static const T neutral_value = 0;

		void operator()(T& a, const T& b) const {
			a -= b;
		}
	};

	template<typename T>
	struct zeta_multiplicative_operation {

		static const T neutral_value = 1;

		void operator()(T& a, const T& b) const {
			a *= b;
		}
	};

	template<typename T>
	struct mobius_multiplicative_operation {

		static const T neutral_value = 1;

		void operator()(T& a, const T& b) const {
			a /= b;
		}
	};

	template<typename T, size_t N, class inclusion, class value_inplace_operation>
	struct try_linear_focal_points_computation {
		bool operator()(
				const powerset_btree<T, N>& support,
				powerset_btree<T, N>& focal_points_tree
		){
			const std::vector<set_N_value<T, N>* >& elements = support.elements();
			for (size_t i = 0; i < elements.size(); ++i){
				focal_points_tree.insert(elements[i]->set, elements[i]->value);
			}

			const std::bitset<N>& absorbing_set = inclusion::absorbing_set_for_operation();
			const std::bitset<N>& dual_absorbing_set = inclusion::absorbing_set_for_dual_operation();

			std::bitset<N> U = elements[0]->set;
			DEBUG(std::clog << "\nU = "<< elements[0]->set;);

			for (size_t i = 1; i < elements.size(); ++i) {
				const std::bitset<N>& A = elements[i]->set;
				if (A != dual_absorbing_set){
					DEBUG(std::clog << "\nI = set_operation(U, "<< A << ")\n";);
					const std::bitset<N>& I = inclusion::set_operation(U, A);

					const set_N_value<T, N>*& newly_inserted_address = focal_points_tree.insert_or_update_if_null(
						I,
						value_inplace_operation::neutral_value
					);
					if (newly_inserted_address && newly_inserted_address->cardinality > 1){
						DEBUG(std::clog << newly_inserted_address->cardinality << "=> Linear focal points computation aborted.\n";);
						return false;
					}
					U = inclusion::set_dual_operation(U, A);
				}
			}
			const set_N_value<T, N>*& newly_inserted_address = focal_points_tree.insert_or_update_if_null(
					absorbing_set,
				value_inplace_operation::neutral_value
			);
			return true;
		}
	};

	template<typename T, size_t N, class inclusion, class value_inplace_operation>
	struct zeta_transformation {

		static inline size_t subgraph_index(const size_t& n, const size_t& i){
			return n - i;
		}

		static inline size_t subgraph_dual_index(const size_t& n, const size_t& i){
			return i;
		}

		static void execute_direct_transformation(
				powerset_btree<T, N>& focal_points_tree
		){
			//clock_t t;
			//t = clock();
			powerset_btree<T, N> focal_points_N_initial_values(focal_points_tree);
			/*
			std::function<T(const std::bitset<N>&)> elements_related_to;
			if(order_relation == order_relation_t::subset){
				elements_related_to = focal_points_N_initial_values.subsets_of;
			}else{
				elements_related_to = focal_points_N_initial_values.supersets_of;
			}*/
			const std::vector<set_N_value<T, N>* >& focal_points = focal_points_N_initial_values.elements();
			T val;
			for (size_t i = 0; i < focal_points.size(); ++i) {
				val = focal_points[i]->value;
				const std::vector<set_N_value<T, N>* >& elements = inclusion::elements_related_to(
					focal_points_N_initial_values,
					focal_points[i]->set
				);
				DEBUG(std::clog << "Elements related to " << focal_points[i]->set << " : \n";);
				for (size_t ii = 0; ii < elements.size(); ++ii) {
					DEBUG(std::clog << elements[ii]->set << std::endl;);
					if (elements[ii]->set != focal_points[i]->set){
						value_inplace_operation(val, elements[ii]->value);
					}
				}
				focal_points_tree.update_or_insert(focal_points[i]->set, val);
			}
			//t = clock() - t;
			//std::cout << (((float) t)/CLOCKS_PER_SEC) << std::endl;
		}

		static inline void consonant_operation(T& value, T& preceding_value){
			value_inplace_operation(value, preceding_value);
			preceding_value = value;
		}

		static void execute_consonant_transformation(
				powerset_btree<T, N>& support
		) {
			T value, preceding_value = value_inplace_operation::neutral_value;
			/*
			std::binary_function<size_t, size_t, bool> comp;
			if(order_relation == order_relation_t::subset){
				comp = std::less<size_t>();
			}else{
				comp = std::greater<size_t>();
			}*/
//			const std::binary_function<size_t, size_t, bool>& comp = card_map_comparator();
//			std::map<size_t, std::vector<set_N_value<T, N>* >, comp > support_card_map = support.elements_by_set_cardinality(comp);
			const auto& support_card_map = inclusion::card_mapping(support);

			for (const auto& c_support_elements : support_card_map) {
				const std::vector<set_N_value<T, N>* >& support_elements = c_support_elements.second;
				consonant_operation(support_elements[0]->value, preceding_value);
				/*
				value = range_binary_operator(set_value->value, preceding_value);
				if (transform_type == transform_type_t::zeta){
					set_value->value = value;
					preceding_value = set_value->value;
				}else{
					preceding_value = set_value->value;
					set_value->value = value;
				}*/
			}
		}
	};

	template<typename T, size_t N, class inclusion, class value_inplace_operation>
	struct mobius_transformation {

		static inline size_t subgraph_index(const size_t& n, const size_t& i){
			return i;
		}

		static inline size_t subgraph_dual_index(const size_t& n, const size_t& i){
			return n - i;
		}

		static void execute_direct_transformation(
				powerset_btree<T, N>& focal_points_tree
		){
			//clock_t t;
			//t = clock();
			/*
			std::binary_function<size_t, size_t, bool> comp;
			std::function<T(const std::bitset<N>&)> elements_related_to;
			if(order_relation == order_relation_t::subset){
				comp = std::less<size_t>();
				elements_related_to = focal_points_tree.subsets_of;
			}else{
				comp = std::greater<size_t>();
				elements_related_to = focal_points_tree.supersets_of;
			}
			*/
//			const std::binary_function<size_t, size_t, bool>& comp = this->card_map_comparator();
//			std::map<size_t, std::vector<set_N_value<T, N>* >, comp > focal_points_card_map = focal_points_tree.elements_by_set_cardinality(comp);
			const auto& focal_points_card_map = inclusion::card_mapping(focal_points_tree);
			T val;
			for (const auto& c_focal_points : focal_points_card_map) {
				const std::vector<std::bitset<N> >& focal_points = c_focal_points.second;
				for (size_t i = 0; i < focal_points.size(); ++i) {
					val = focal_points[i]->value;
					const std::vector<set_N_value<T, N>* >& elements = inclusion::elements_related_to(focal_points[i]->set);
					for (size_t ii = 0; ii < elements.size(); ++ii) {
						if (elements[ii]->set != focal_points[i]->set){
							value_inplace_operation(val, elements[ii]->value);
						}
					}
					focal_points_tree.insert(focal_points[i]->set, val);
				}
			}
			//t = clock() - t;
			//std::cout << (((float) t)/CLOCKS_PER_SEC) << std::endl;
		}

		static inline void consonant_operation(T& value, T& preceding_value){
			T old_value = value;
			value_inplace_operation(value, preceding_value);
			preceding_value = old_value;
		}

		static void execute_consonant_transformation(
				powerset_btree<T, N>& support
		) {
			T value, preceding_value = value_inplace_operation::neutral_value;
			/*
			std::binary_function<size_t, size_t, bool> comp;
			if(order_relation == order_relation_t::subset){
				comp = std::less<size_t>();
			}else{
				comp = std::greater<size_t>();
			}*/
//			const std::binary_function<size_t, size_t, bool>& comp = card_map_comparator();
//			std::map<size_t, std::vector<set_N_value<T, N>* >, comp > support_card_map = support.elements_by_set_cardinality(comp);
			const auto& support_card_map = inclusion::card_mapping(support);

			for (const auto& c_support_elements : support_card_map) {
				const std::vector<set_N_value<T, N>* >& support_elements = c_support_elements.second;
				consonant_operation(support_elements[0]->value, preceding_value);
				/*
				value = range_binary_operator(set_value->value, preceding_value);
				if (transform_type == transform_type_t::zeta){
					set_value->value = value;
					preceding_value = set_value->value;
				}else{
					preceding_value = set_value->value;
					set_value->value = value;
				}*/
			}
		}
	};

	template<typename T, class operation>
	struct FMT_operation_up_inclusion {

		void operator()(const T& a, T& b) const {
			operation(b, a);
		}
	};

	template<typename T, class operation>
	struct FMT_operation_down_inclusion {

		void operator()(T& a, const T& b) const {
			operation(a, b);
		}
	};

	template<
		typename T,
		size_t N,
		class inclusion,
		class value_inplace_operation,
		class FMT_value_inplace_operation
	>
	struct FMT {

		static powerset_btree<T, N> execute_FMT_reduced_to_core(
				const powerset_btree<T, N>& support
		) {
			const std::vector<set_N_value<T, N>* >& support_elements = support.elements();
			std::bitset<N> core = 0;
			for(size_t i = 0; i < support_elements.size(); ++i){
				core |= support_elements[i]->set;
			}

			//			size_t reduced_powerset_size = pow(2, core.count());
			powerset_btree<T, N> transform(support.get_FOD(), pow(2, core.count()));
			transform.copy(support);
//			powerset_btree<T, N> transform(support.get_FOD(), reduced_powerset_size);
			std::vector<set_N_value<T, N>* > focal_points = transform.elements();
//			std::unordered_map<std::bitset<N>, set_N_value<T, N>* > focal_points_map;
//			focal_points.reserve(reduced_powerset_size);
//			focal_points_map.reserve(reduced_powerset_size);
//
//			for (size_t i = 0; i < support_elements.size(); ++i){
//				focal_points.emplace_back(support_elements[i]->set);
//				focal_points_map.emplace(support_elements[i]->set, transform.insert(support_elements[i]->set, support_elements[i]->value));
//			}

			size_t i = core._Find_first();
			while(i < N){
				for (size_t e = 0; e < focal_points.size(); ++e) {
					std::bitset<N> new_set = (const std::bitset<N>&) focal_points[e]->set;
					new_set.set(i, true);
					set_N_value<T, N>* node = transform.find(new_set);
					if (!node){
						transform.insert(new_set, value_inplace_operation::neutral_value);
					}
				}
				i = core._Find_next(i);
			}

			std::vector<set_N_value<T, N>* > powerset_elements = transform.elements();

			i = core._Find_first();
			while(i < N){
				for (size_t e = 0; e < powerset_elements.size(); ++e){
					std::bitset<N> set_B = (const std::bitset<N>&) powerset_elements[e]->set;
					set_B.set(i, true);

					if (set_B != powerset_elements[e]->set){
						set_N_value<T, N>* B = transform.find(set_B);

						if (B){
							FMT_value_inplace_operation(B->second->value, powerset_elements[e]->value);
						}
					}
				}
				i = core._Find_next(i);
			}
			return transform;
		}

		static std::vector<T> execute_FMT(
				const std::vector<T>& powerset_values
		) {
			if(powerset_values.size() != pow(2, N)){
				std::cerr << "\nThe size of the given vector is not 2^N, where N is the given size corresponding to the considered FOD.\n";
				return powerset_values;
			}

			std::vector<T> transform(powerset_values);
			size_t sub_powerset_size, sub_powerset_dual_size, index;
			for (size_t i = 1; i <= N; ++i){

				sub_powerset_size = pow(2, i);
				for (size_t j = 1; j <= sub_powerset_size; j += 2){

					sub_powerset_dual_size = pow(2, N - i);
					for (size_t k = 0; k <= sub_powerset_dual_size-1; ++k){
						index = (j-inclusion::target_index_offset_FMT()) * sub_powerset_dual_size + k;
						value_inplace_operation(transform[index], transform[(j-inclusion::source_index_offset_FMT()) * sub_powerset_dual_size + k]);
					}
				}
			}
			return transform;
		}
	};


	template <
		typename T,
		size_t N,
		class transformation,
		class inclusion,
		class value_inplace_operation
	>
	struct EMT {

		static void build_bridge_map(
				std::unordered_map<std::bitset<N>, set_N_value<T, N>* >& bridge_map,
				const powerset_btree<T, N>& focal_points_tree,
				const std::vector<std::bitset<N> >& iota_sequence
		) {
			const std::vector<set_N_value<T, N>* >& focal_points = focal_points_tree.elements();

			powerset_btree<bool, N> proxies_missing_targets(focal_points_tree.get_FOD(), iota_sequence.size());

			for (size_t i = 0; i < iota_sequence.size(); ++i) {
				for (size_t e = 0; e < focal_points.size(); ++e){
//					const std::bitset<N>& set = focal_points[e]->set | iota_sequence[i];
					const std::bitset<N>& set = inclusion::set_dual_operation(focal_points[e]->set, iota_sequence[i]);
					if (!focal_points_tree.find(set)){
						proxies_missing_targets.insert(set, true);
					}
				}
			}
			if (proxies_missing_targets.size() == 0){
				return;
			}

//			const std::binary_function<size_t, size_t, bool>& comp = std::less<size_t>();
//			const std::binary_function<size_t, size_t, bool>& comp = card_map_dual_comparator();
//			std::map<size_t, std::vector<set_N_value<T, N>* >, comp > focal_points_card_map = focal_points_tree.elements_by_set_cardinality(comp);
			const auto& focal_points_card_map = inclusion::card_mapping_dual(focal_points_tree);
			//clock_t t;
			//t = clock();
			size_t nb_targets_to_find, cc = 0;
			for (const auto& c_focal_points : focal_points_card_map) {
				for (; cc < c_focal_points.first; ++cc){
					nb_targets_to_find += proxies_missing_targets.get_nb_sets_of_cardinality(cc);
				}
				if (nb_targets_to_find > 0){
					const std::vector<set_N_value<T, N>* >& focal_points_with_same_size = c_focal_points.second;

					for (size_t i = 0; i < focal_points_with_same_size.size(); ++i){
//						const std::vector<set_N_value<bool, N>* >& subsets = proxies_missing_targets.subsets_of(focal_points_with_same_size[i]->set);
						const std::vector<set_N_value<bool, N>* >& proxies = inclusion::elements_dually_related_to(proxies_missing_targets, focal_points_with_same_size[i]->set);

						for (size_t s = 0; s < proxies.size(); ++s){
							bridge_map[proxies[s]->set] = focal_points_with_same_size[i];
							proxies_missing_targets.nullify(proxies[s]);
							--nb_targets_to_find;
						}
						if (nb_targets_to_find == 0){
							break;
						}
					}
				}
			}
			//t = clock() - t;
			//std::cout << "missing proxy search : " << (((float) t)/CLOCKS_PER_SEC) << " ";
		}


		static void execute_EMT_with_semilattice(
				powerset_btree<T, N>& focal_points_tree,
				const std::vector<std::bitset<N> >& iota_sequence
		) {
			clock_t t;

			if (iota_sequence.size() == 0)
				return;

			std::vector<std::bitset<N> > sync_sequence;
			sync_sequence.reserve(iota_sequence.size());

			sync_sequence.emplace_back(iota_sequence[0]);
			for (size_t i = 1; i < iota_sequence.size(); ++i){
//				sync_sequence.emplace_back(sync_sequence[i-1] | iota_sequence[i]);
				sync_sequence.emplace_back(inclusion::set_dual_operation(sync_sequence[i-1], iota_sequence[i]));
			}
			std::unordered_map<std::bitset<N>, set_N_value<T, N>* > bridge_map;
			build_bridge_map(bridge_map, focal_points_tree, iota_sequence);

			//t = clock();
			size_t nb_iota = iota_sequence.size()-1;
//			size_t iota_index;
			const std::vector<set_N_value<T, N>* >& focal_points = focal_points_tree.elements();
			for (size_t i = 0; i < iota_sequence.size(); ++i){
//				if (transform_type == transform_type_t::zeta)
//					iota_index = i;
//				else
//					iota_index = nb_iota - i;
				const size_t& iota_index = transformation::subgraph_dual_index(nb_iota, i);

				for (size_t e = 0; e < focal_points.size(); ++e){
//					const std::bitset<N>& proxy = focal_points[e]->set | iota_sequence[iota_index];
					const std::bitset<N>& proxy = inclusion::set_dual_operation(focal_points[e]->set, iota_sequence[iota_index]);

					if (focal_points[e]->set != proxy){
						const set_N_value<T, N>* coupled_set = bridge_map[proxy];

//						if (coupled_set && FOD<N>::is_subset_of(coupled_set->set, focal_points[e]->set | sync_sequence[iota_index])){
						if (coupled_set && FOD<N>::is_subset_of(coupled_set->set, inclusion::set_dual_operation(focal_points[e]->set, sync_sequence[iota_index]))){
							value_inplace_operation(focal_points[e]->value, coupled_set->value);
						}
					}
				}
			}
			//t = clock() - t;
			//std::cout << (((float) t)/CLOCKS_PER_SEC) << std::endl;
		}


		static void execute_EMT_with_lattice(
				powerset_btree<T, N>& lattice_support,
				const std::vector<std::bitset<N> >& iota_sequence
		) {
			const std::vector<set_N_value<T, N>* >& lattice_support_elements = lattice_support.elements();

			size_t nb_iota = iota_sequence.size()-1;
//			size_t iota_index;
			for (size_t i = 0; i < iota_sequence.size(); ++i){
//				if (transform_type == transform_type_t::Mobius)
//					iota_index = i;
//				else
//					iota_index = nb_iota - i;
				const size_t& iota_index = transformation::subgraph_index(nb_iota, i);

				for (size_t e = 0; e < lattice_support_elements.size(); ++e){
//					const std::bitset<N>& set_B = lattice_support_elements[e]->set | iota_sequence[iota_index];
					const std::bitset<N>& set_B = inclusion::set_operation(lattice_support_elements[e]->set, iota_sequence[iota_index]);

					if (set_B != lattice_support_elements[e]->set){
						set_N_value<T, N>* B = lattice_support.find(set_B);

						if (B){
							value_inplace_operation(B->value, lattice_support_elements[e]->value);
						}
					}
				}
			}
		}

		static const std::vector<std::bitset<N> >& build_semilattice_support(
				const powerset_btree<T, N>& support,
				powerset_btree<T, N>& focal_points_tree
		) {
			std::vector<std::bitset<N> > focal_points;
			const std::vector<set_N_value<T, N>* >& support_elements = support.elements();
			focal_points.reserve(support.size());

			for (size_t i = 0; i < support_elements.size(); ++i){
				focal_points.emplace_back(support_elements[i]->set);
				focal_points_tree.insert(support_elements[i]->set, support_elements[i]->value);
			}

			for (size_t i = 0; i < support_elements.size(); ++i){
				size_t end = focal_points.size();
				for (size_t ii = i+1; ii < end; ++ii){
					const std::bitset<N>& focal_point = inclusion::set_operation(support_elements[i]->set, focal_points[ii]);
					const set_N_value<T, N>*& newly_inserted_address = focal_points_tree.insert_or_update_if_null(
						focal_point,
						value_inplace_operation::neutral_value
					);
					if (newly_inserted_address){
						focal_points.emplace_back(focal_point);
					}
				}
			}
			return compute_iota_elements_dual(
				support
			);
		}


		/*
		 * In this function, this->iota_sequence is supposed to contain all regular iota elements (i.e. join-irreducible of this lattice).
		 */
		static const std::vector<std::bitset<N> >& build_truncated_lattice_support(
				const powerset_btree<T, N>& support,
				powerset_btree<T, N>& cropped_lattice_support
		) {
			const std::vector<std::bitset<N> >& iota_sequence = compute_iota_elements(
				support
			);

			std::vector<std::bitset<N> > focal_points;
			focal_points.reserve(support.size());

			const std::vector<set_N_value<T, N>* >& elements = support.elements();
			for (size_t i = 0; i < elements.size(); ++i){
				focal_points.emplace_back(elements[i]->set);
				cropped_lattice_support.insert(elements[i]->set, elements[i]->value);
			}

			for (size_t i = 0; i < iota_sequence.size(); ++i) {
				size_t end = focal_points.size();
				for (size_t ii = 0; ii < end; ++ii) {
					const std::bitset<N>& new_set = inclusion::set_operation(iota_sequence[i], focal_points[ii]);
					const set_N_value<T, N>*& newly_inserted_address = cropped_lattice_support.insert_or_update_if_null(
						new_set,
						value_inplace_operation::neutral_value
					);
					if (newly_inserted_address){
						focal_points.emplace_back(new_set);
					}
				}
			}
			DEBUG({
				std::clog << "\nCropped lattice support: \n";
				cropped_lattice_support.print(std::clog);
			});
			return iota_sequence;
		}
	};
}		// namespace efficient_DST

#endif // EFFICIENT_DST_MOBIUS_INVERSION_TEMPLATE_HPP
