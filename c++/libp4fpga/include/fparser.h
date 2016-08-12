/*
  Copyright 2015-2016 P4FPGA Project

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef _BACKENDS_FPGA_FPGAPARSER_H_
#define _BACKENDS_FPGA_FPGAPARSER_H_

#include "ir/ir.h"
#include "fprogram.h"
#include "ftype.h"
#include "bsvprogram.h"

namespace FPGA {

  inline void append_format(BSVProgram & bsv, const char* fmt, ...)
  {
    bsv.getParserBuilder().emitIndent();	
    va_list args;
    va_start(args,fmt);
    bsv.getParserBuilder().appendFormat(fmt, args);	
    va_end(args);
    bsv.getParserBuilder().newline();
  }

  inline void append_line(BSVProgram & bsv, const char* fmt, ...)
  {
    bsv.getParserBuilder().emitIndent();	
    va_list args;    
    va_start(args,fmt);
    bsv.getParserBuilder().appendLine((boost::format("fmt") % args).str().c_str());	
    va_end(args);

  }
 
  inline void incr_indent(BSVProgram & bsv)
  {
    bsv.getParserBuilder().increaseIndent();
  }

  inline void decr_indent(BSVProgram & bsv)
  {
    bsv.getParserBuilder().decreaseIndent();
  }

  
  class FPGAParserState : public FPGAObject {
  public:
    const IR::ParserState* state;
    const FPGAParser* parser;

  FPGAParserState(const IR::ParserState* state, FPGAParser* parser) :
    state(state), parser(parser) {}
    void emit(BSVProgram & bsv) override;
  };

  class FPGAParser : public FPGAObject {
  protected:
    // TODO(rjs): I think these should be const
    void emitTypes(BSVProgram & bsv);
    void emitParseState(BSVProgram & bsv);
    void emitInterface(BSVProgram & bsv);
    void emitFunctVerbosity(BSVProgram & bsv);
    void emitModule(BSVProgram & bsv);

    std::vector<IR::BSV::Reg*>          reg;
    std::vector<IR::BSV::CReg*>         creg;
    std::vector<IR::BSV::PulseWireOR*>  wires;
    std::vector<IR::BSV::Rule*>         rules;

  public:
    const FPGAProgram*            program;
    const P4::TypeMap*            typeMap;
    const IR::ParserBlock*        parserBlock;
    std::vector<FPGAParserState*> states;
    const IR::Parameter*          packet;
    const IR::Parameter*          headers;
    const IR::Parameter*          userMetadata;
    const IR::Parameter*          stdMetadata;
    FPGAType*                     headerType;

    explicit FPGAParser(const FPGAProgram* program,
			const IR::ParserBlock* block,
                        const P4::TypeMap* typeMap);

    void emit(BSVProgram & bsv) override;
    bool build();
  };

}  // namespace FPGA

#endif /* _BACKENDS_FPGA_FPGAPARSER_H_ */
