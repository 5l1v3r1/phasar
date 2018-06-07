/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * Gen.h
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_FLOW_FUNC_GEN_H_
#define ANALYSIS_IFDS_IDE_FLOW_FUNC_GEN_H_

#include <phasar/PhasarLLVM/IfdsIde/FlowFunction.h>
#include <set>

namespace psr {

template <typename D> class Gen : public FlowFunction<D> {
private:
  D genValue;
  D zeroValue;

public:
  Gen(D genValue, D zeroValue) : genValue(genValue), zeroValue(zeroValue) {}
  virtual ~Gen() = default;
  std::set<D> computeTargets(D source) override {
    if (source == zeroValue)
      return {source, genValue};
    else
      return {source};
  }
};

} // namespace psr

#endif /* ANALYSIS_IFDS_IDE_FLOW_FUNC_GEN_HH_ */
