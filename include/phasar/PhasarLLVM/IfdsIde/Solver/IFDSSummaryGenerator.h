/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * IFDSSummaryGenerator.h
 *
 *  Created on: 17.05.2017
 *      Author: philipp
 */

#ifndef SRC_ANALYSIS_IFDS_IDE_IFDSSUMMARYGENERATOR_H_
#define SRC_ANALYSIS_IFDS_IDE_IFDSSUMMARYGENERATOR_H_

#include "../../../utils/utils.h"
#include "../../misc/SummaryStrategy.h"
#include "../FlowFunction.h"
#include "../ZeroValue.h"
#include "../flow_func/GenAll.h"
#include <iostream>
#include <set>
#include <vector>
using namespace std;
using namespace psr;
namespace psr {

template <typename N, typename D, typename M, typename I,
          typename ConcreteTabulationProblem, typename ConcreteSolver>
class IFDSSummaryGenerator {
protected:
  const M toSummarize;
  const I icfg;
  const SummaryGenerationStrategy CTXStrategy;

  virtual vector<D> getInputs() = 0;
  virtual vector<bool> generateBitPattern(const vector<D> &inputs,
                                          const set<D> &subset) = 0;

  class CTXFunctionProblem : public ConcreteTabulationProblem {
  public:
    const N start;
    set<D> facts;

    CTXFunctionProblem(N start, set<D> facts, I icfg)
        : ConcreteTabulationProblem(icfg), start(start), facts(facts) {
      this->solver_config.followReturnsPastSeeds = false;
      this->solver_config.autoAddZero = true;
      this->solver_config.computeValues = true;
      this->solver_config.recordEdges = false;
      this->solver_config.computePersistedSummaries = false;
    }

    virtual map<N, set<D>> initialSeeds() override {
      map<N, set<D>> seeds;
      seeds.insert(make_pair(start, facts));
      return seeds;
    }
  };

public:
  IFDSSummaryGenerator(M Function, I icfg, SummaryGenerationStrategy Strategy)
      : toSummarize(Function), icfg(icfg), CTXStrategy(Strategy) {}
  virtual ~IFDSSummaryGenerator() = default;
  virtual set<pair<vector<bool>, shared_ptr<FlowFunction<D>>>>
  generateSummaryFlowFunction() {
    set<pair<vector<bool>, shared_ptr<FlowFunction<D>>>> summary;
    vector<D> inputs = getInputs();
    set<D> inputset;
    inputset.insert(inputs.begin(), inputs.end());
    set<set<D>> InputCombinations;
    // initialize the input combinations that should be considered
    switch (CTXStrategy) {
    case SummaryGenerationStrategy::always_all:
      InputCombinations.insert(inputset);
      break;
    case SummaryGenerationStrategy::always_none:
      InputCombinations.insert(set<D>());
      break;
    case SummaryGenerationStrategy::all_and_none:
      InputCombinations.insert(inputset);
      InputCombinations.insert(set<D>());
      break;
    case SummaryGenerationStrategy::powerset:
      InputCombinations = computePowerSet(inputset);
      break;
    case SummaryGenerationStrategy::all_observed:
      // TODO here we have to track what we have already observed first!
      break;
    }
    for (auto subset : InputCombinations) {
      cout << "Generate summary for specific context: "
           << generateBitPattern(inputs, subset) << "\n";
      CTXFunctionProblem functionProblem(
          *icfg.getStartPointsOf(toSummarize).begin(), subset, icfg);
      ConcreteSolver solver(functionProblem, true);
      solver.solve();
      // get the result at the end of this function and
      // create a flow function from this set using the GenAll class
      set<N> LastInsts = icfg.getExitPointsOf(toSummarize);
      set<D> results;
      for (auto fact : solver.resultsAt(*LastInsts.begin())) {
        results.insert(fact.first);
      }
      summary.insert(
          make_pair(generateBitPattern(inputs, subset),
                    make_shared<GenAll<D>>(results, ZeroValue::getInstance())));
    }
    return summary;
  }
};

} // namespace psr

#endif
