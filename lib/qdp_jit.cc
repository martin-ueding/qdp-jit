#include "qdp.h"



namespace QDP {

  int jit_block::count = 0;

  jit_function_t jit_internal_function;
  jit_block_t    jit_internal_block;

  std::ostream& operator<< (std::ostream& stream, const jit_state_space& space ) {
    stream << get_state_space_str(space);
    return stream;
  }

  namespace PTX {
    // { "f32","f64","u16","u32","u64","s16","s32","s64", "u8","b16","b32","b64","pred" };
    // { "f"  ,"d"  ,"h"  ,"u"  ,"w"  ,"q"  ,"i"  ,"l"  ,"s"  ,"x"  ,"y"  ,"z"  ,"p" };
    // { ""   ,""   ,"lo.","lo.","lo.","lo.","lo.","lo.","lo.",""   ,""   ,""   ,"" };

    // LLVM type 
    std::map< jit_llvm_builtin , std::array<const char*,1> > create_ptx_type_matrix() {
      std::map< jit_llvm_builtin , std::array<const char*,1> > ptx_type_matrix {
	{jit_llvm_builtin::i1 ,{{"i1" }}},
	  {jit_llvm_builtin::i32 ,{{"i32" }}},
	    {jit_llvm_builtin::i64 ,{{"i64" }}},
	      {jit_llvm_builtin::flt ,{{"float" }}},
		{jit_llvm_builtin::dbl ,{{"double" }}} };
      return ptx_type_matrix;
    }

    std::map< jit_llvm_builtin , std::array<const char*,1> > ptx_type_matrix;
	
    // std::map< jit_state_space , const char * > create_state_space_map()
    // {
    //   std::map< jit_state_space , const char * > map_state_space_map;
    //   map_state_space_map[ jit_state_space::state_default] = "default";
    //   map_state_space_map[ jit_state_space::state_global]  = "global";
    //   map_state_space_map[ jit_state_space::state_local]   = "local";
    //   map_state_space_map[ jit_state_space::state_shared]  = "shared";
    //   return map_state_space_map;
    // }

    std::map< jit_llvm_builtin , std::map<jit_llvm_builtin,const char *> > create_cvt_from_to()
    {
      std::map< jit_llvm_builtin , std::map<jit_llvm_builtin,const char *> > map_cvt_from_to;
      map_cvt_from_to[jit_llvm_builtin::i1][jit_llvm_builtin::float] = "sitofp";
      map_cvt_from_to[jit_llvm_builtin::i1][jit_llvm_builtin::double] = "sitofp";
      map_cvt_from_to[jit_llvm_builtin::i1][jit_llvm_builtin::i64] = "sext";
      map_cvt_from_to[jit_llvm_builtin::i1][jit_llvm_builtin::i32] = "sext";

      map_cvt_from_to[jit_llvm_builtin::i32][jit_llvm_builtin::float] = "sitofp";
      map_cvt_from_to[jit_llvm_builtin::i32][jit_llvm_builtin::double] = "sitofp";
      map_cvt_from_to[jit_llvm_builtin::i32][jit_llvm_builtin::i64] = "sext";
      map_cvt_from_to[jit_llvm_builtin::i32][jit_llvm_builtin::i1] = "trunc";

      map_cvt_from_to[jit_llvm_builtin::i64][jit_llvm_builtin::float] = "sitofp";
      map_cvt_from_to[jit_llvm_builtin::i64][jit_llvm_builtin::double] = "sitofp";
      map_cvt_from_to[jit_llvm_builtin::i64][jit_llvm_builtin::i32] = "trunc";
      map_cvt_from_to[jit_llvm_builtin::i64][jit_llvm_builtin::i1] = "trunc";

      map_cvt_from_to[jit_llvm_builtin::float][jit_llvm_builtin::double] = "fpext";
      map_cvt_from_to[jit_llvm_builtin::float][jit_llvm_builtin::i32] = "fptosi";
      map_cvt_from_to[jit_llvm_builtin::float][jit_llvm_builtin::i64] = "fptosi";

      map_cvt_from_to[jit_llvm_builtin::double][jit_llvm_builtin::float] = "fptrunc";
      map_cvt_from_to[jit_llvm_builtin::double][jit_llvm_builtin::i32] = "fptosi";
      map_cvt_from_to[jit_llvm_builtin::double][jit_llvm_builtin::i64] = "fptosi";

      return map_cvt_from_to;
    }
    const std::map< jit_llvm_builtin , 
		    std::map<jit_llvm_builtin,const char *> > map_cvt_from_to = create_cvt_from_to();


    std::map< jit_llvm_builtin , std::map<jit_llvm_builtin,jit_llvm_builtin> > create_promote()
    {
      std::map< jit_llvm_builtin , std::map<jit_llvm_builtin,jit_llvm_builtin> > map_promote;
      map_promote[jit_llvm_builtin::i1][jit_llvm_builtin::flt] = jit_llvm_builtin::flt;
      map_promote[jit_llvm_builtin::i1][jit_llvm_builtin::dbl] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::i1][jit_llvm_builtin::i32] = jit_llvm_builtin::i32;
      map_promote[jit_llvm_builtin::i1][jit_llvm_builtin::i64] = jit_llvm_builtin::i64;

      map_promote[jit_llvm_builtin::i32][jit_llvm_builtin::flt] = jit_llvm_builtin::flt;
      map_promote[jit_llvm_builtin::i32][jit_llvm_builtin::dbl] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::i32][jit_llvm_builtin::i1] = jit_llvm_builtin::i32;
      map_promote[jit_llvm_builtin::i32][jit_llvm_builtin::i64] = jit_llvm_builtin::i64;

      map_promote[jit_llvm_builtin::i64][jit_llvm_builtin::flt] = jit_llvm_builtin::flt;
      map_promote[jit_llvm_builtin::i64][jit_llvm_builtin::dbl] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::i64][jit_llvm_builtin::i1] = jit_llvm_builtin::i64;
      map_promote[jit_llvm_builtin::i64][jit_llvm_builtin::i32] = jit_llvm_builtin::i64;

      map_promote[jit_llvm_builtin::flt][jit_llvm_builtin::dbl] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::flt][jit_llvm_builtin::i1] = jit_llvm_builtin::flt;
      map_promote[jit_llvm_builtin::flt][jit_llvm_builtin::i32] = jit_llvm_builtin::flt;
      map_promote[jit_llvm_builtin::flt][jit_llvm_builtin::i64] = jit_llvm_builtin::flt;

      map_promote[jit_llvm_builtin::dbl][jit_llvm_builtin::flt] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::dbl][jit_llvm_builtin::i1] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::dbl][jit_llvm_builtin::i32] = jit_llvm_builtin::dbl;
      map_promote[jit_llvm_builtin::dbl][jit_llvm_builtin::i64] = jit_llvm_builtin::dbl;

      return map_promote;
    }
    std::map< jit_llvm_builtin , jit_llvm_builtin > create_wide_promote()
    {
      std::map< jit_llvm_builtin , jit_llvm_builtin > map_wide_promote;
      // map_wide_promote[ jit_llvm_builtin::f32 ] = jit_llvm_builtin::f64;
      // map_wide_promote[ jit_llvm_builtin::u16 ] = jit_llvm_builtin::u32;
      // map_wide_promote[ jit_llvm_builtin::u32 ] = jit_llvm_builtin::u64;
      // map_wide_promote[ jit_llvm_builtin::u64 ] = jit_llvm_builtin::u64;
      // map_wide_promote[ jit_llvm_builtin::s32 ] = jit_llvm_builtin::s64;
      // map_wide_promote[ jit_llvm_builtin::s64 ] = jit_llvm_builtin::s64;
      return map_wide_promote;
    }
    std::map< jit_llvm_builtin , jit_llvm_builtin > create_bit_type()
    {
      std::map< jit_llvm_builtin , jit_llvm_builtin > map_bit_type;
      // map_bit_type[ jit_llvm_builtin::u32 ] = jit_llvm_builtin::b32;
      // map_bit_type[ jit_llvm_builtin::s32 ] = jit_llvm_builtin::b32;
      // map_bit_type[ jit_llvm_builtin::f32 ] = jit_llvm_builtin::b32;
      // map_bit_type[ jit_llvm_builtin::u64 ] = jit_llvm_builtin::b64;
      // map_bit_type[ jit_llvm_builtin::s64 ] = jit_llvm_builtin::b64;
      // map_bit_type[ jit_llvm_builtin::f64 ] = jit_llvm_builtin::b64;
      // map_bit_type[ jit_llvm_builtin::u16 ] = jit_llvm_builtin::b16;
      // map_bit_type[ jit_llvm_builtin::s16 ] = jit_llvm_builtin::b16;
      // map_bit_type[ jit_llvm_builtin::pred ] = jit_llvm_builtin::pred;
      return map_bit_type;
    }
    std::map< jit_state_space , 
	      std::map< jit_state_space , 
			jit_state_space > >
    create_state_promote()
    {
      std::map< jit_state_space , 
		std::map< jit_state_space , 
			  jit_state_space > > map_state_promote;
      // map_state_promote[ jit_state_space::state_default ][ jit_state_space::state_shared ] = jit_state_space::state_shared;
      // map_state_promote[ jit_state_space::state_shared ][ jit_state_space::state_default ] = jit_state_space::state_shared;
      // map_state_promote[ jit_state_space::state_default ][ jit_state_space::state_global ] = jit_state_space::state_global;
      // map_state_promote[ jit_state_space::state_global ][ jit_state_space::state_default ] = jit_state_space::state_global;
      // map_state_promote[ jit_state_space::state_default ][ jit_state_space::state_local ] = jit_state_space::state_local;
      // map_state_promote[ jit_state_space::state_local ][ jit_state_space::state_default ] = jit_state_space::state_local;

      // map_state_promote[ jit_state_space::state_shared ][ jit_state_space::state_shared ] = jit_state_space::state_shared;
      // map_state_promote[ jit_state_space::state_shared ][ jit_state_space::state_global ] = jit_state_space::state_shared;
      // map_state_promote[ jit_state_space::state_global ][ jit_state_space::state_shared ] = jit_state_space::state_shared;
      // map_state_promote[ jit_state_space::state_local  ][ jit_state_space::state_local  ] = jit_state_space::state_local;
      // map_state_promote[ jit_state_space::state_local  ][ jit_state_space::state_global ] = jit_state_space::state_local;
      // map_state_promote[ jit_state_space::state_global ][ jit_state_space::state_local  ] = jit_state_space::state_local;
      // map_state_promote[ jit_state_space::state_global ][ jit_state_space::state_global ] = jit_state_space::state_global;
      return map_state_promote;
    }
    const std::map< jit_state_space , const char * > map_state_space = create_state_space_map();
    const std::map< int , std::pair<const char *,
				    std::string> >     map_ptx_math_functions_unary = create_ptx_math_functions_unary();
    const std::map< int , std::pair<const char *,
				    std::string> >     map_ptx_math_functions_binary = create_ptx_math_functions_binary();
    const std::map< jit_llvm_builtin , std::map<jit_llvm_builtin,jit_llvm_builtin> >          map_promote            = create_promote();
    const std::map< jit_llvm_builtin , jit_llvm_builtin >                        map_wide_promote       = create_wide_promote();
    const std::map< jit_llvm_builtin , jit_llvm_builtin >                        map_bit_type           = create_bit_type();
    const std::map< jit_state_space , 
		    std::map< jit_state_space , 
			      jit_state_space > > map_state_promote  = create_state_promote();
  }


  int jit_number_of_types() { return PTX::ptx_type_matrix.size(); }

  // const char * get_state_space_str( jit_state_space mem_state ){ 
  //   assert( PTX::map_state_space.count(mem_state) > 0 );
  //   return PTX::map_state_space.at(mem_state); 
  // }
  // const char * jit_get_identifier_local_memory() {
  //   return PTX::jit_identifier_local_memory;
  // }

  const char * jit_get_map_cvt_from_to(jit_llvm_builtin from,jit_llvm_builtin to) {
    static const char * nullstr = "";
    if (!PTX::map_cvt_from_to.count(from))
      return nullstr;
    if (!PTX::map_cvt_from_to.at(from).count(to))
      return nullstr;
    return PTX::map_cvt_from_to.at(from).at(to);
  }


  // jit_llvm_type jit_bit_type(jit_llvm_type type) {
  //   assert( PTX::map_bit_type.count( type ) > 0 );
  //   return PTX::map_bit_type.at( type );
  // }


  jit_llvm_builtin jit_type_promote(jit_llvm_builtin t0,jit_llvm_builtin t1) {
    //std::cout << "type promote: " << t0 << " " << t1 << "\n";
    if (t0==t1) return t0;
    if ( PTX::map_promote.count(t0) == 0 )
      std::cout << "promote: " << jit_get_llvm_builtin(t0) << " " << jit_get_llvm_builtin(t1) << "\n";
    assert( PTX::map_promote.count(t0) > 0 );
    if ( PTX::map_promote.at(t0).count(t1) == 0 )
      std::cout << "promote: " << jit_get_llvm_builtin(t0) << " " << jit_get_llvm_builtin(t1) << "\n";
    assert( PTX::map_promote.at(t0).count(t1) > 0 );
    jit_llvm_builtin ret = PTX::map_promote.at(t0).at(t1);
    //assert((ret >= 0) && (ret < jit_number_of_types()));
    //std::cout << "         ->  " << PTX::ptx_type_matrix.at( ret )[0] << "\n";
    return ret;
  }


  jit_value::jit_value( jit_llvm_type type_ ): 
    ever_assigned(true),
    type(type_)
  {
    reg_alloc();
  }



  jit_value::jit_value( int val ): 
    ever_assigned(true)
  {
    type = jit_llvm_type::i32;
    reg_alloc();
    // Workaround: NVVM has no support for constant assignment
    jit_get_function()->get_prg() << *this << " = "
				  << "add i32 " 
				  << val << ",0\n";
  }


  // jit_value::jit_value( size_t val ): 
  //   ever_assigned(true), 
  //   mem_state(jit_state_space::state_default)
  // {
  //   type = val <= (size_t)std::numeric_limits<int32_t>::max() ? jit_llvm_type::u32 : jit_llvm_type::u64;
  //   reg_alloc();
  //   std::ostringstream oss; oss << val;
  //   jit_ins_mov( *this , oss.str() );
  // }


  jit_value::jit_value( double val ): 
    ever_assigned(true),
    type(jit_type<REAL>::value)
  {
    reg_alloc();
    // Workaround: NVVM has no support for constant assignment
    std::ostringstream oss; 
    oss.setf(ios::scientific);
    oss.precision(std::numeric_limits<double>::digits10 + 1);
    oss << val;
    jit_get_function()->get_prg() << *this << " = "
				  << "fadd " << jit_get_ptx_type(*this) << " "
				  << oss.str() << ",0.0\n";
  }






  jit_state_space jit_state_promote( jit_state_space ss0 , jit_state_space ss1 ) {
    //std::cout << "state_promote: " << ss0 << " " << ss1 << "\n";
    if ( ss0 == ss1 ) return ss0;
    assert( PTX::map_state_promote.count( ss0 ) > 0 );
    assert( PTX::map_state_promote.at( ss0 ).count( ss1 ) > 0 );
    jit_state_space ret = PTX::map_state_promote.at( ss0 ).at( ss1 );
    assert( ret == jit_state_space::state_global || ret == jit_state_space::state_shared || ret == jit_state_space::state_local );
    //std::cout << "         ->  " << PTX::ptx_type_matrix.at( ret )[0] << "\n";
    return ret;
  }


  jit_llvm_type jit_type_wide_promote(jit_llvm_type t0) {
    assert( PTX::map_wide_promote.count(t0) > 0 );
    return PTX::map_wide_promote.at(t0);
  }

  const char * jit_get_llvm_builtin( jit_llvm_builtin type ) {
    assert( PTX::ptx_type_matrix.count(type) > 0 );
    return PTX::ptx_type_matrix.at(type)[0];
  }

  // const char * jit_get_ptx_letter( jit_llvm_type type ) {
  //   assert( PTX::ptx_type_matrix.count(type) > 0 );
  //   return  PTX::ptx_type_matrix.at(type)[1];
  // }

  // const char * jit_get_mul_specifier_lo_str( jit_llvm_type type ) {
  //   assert( PTX::ptx_type_matrix.count(type) > 0 );
  //   return PTX::ptx_type_matrix.at(type)[2];
  // }

  // const char * jit_get_div_specifier( jit_llvm_type type ) {
  //   assert( PTX::ptx_type_matrix.count(type) > 0 );
  //   return PTX::ptx_type_matrix.at(type)[3];
  // }
  


  jit_block_t jit_block_create() {
    return std::make_shared< jit_block >();
  }


  void jit_ins_start_block( jit_block_t& block ) {
    if (!block)
      block = jit_block_create();
    jit_get_function()->get_prg() << *block << ":\n";
  }

  jit_block_t jit_ins_start_new_block() {
    jit_block_t block = jit_block_create();
    jit_get_function()->get_prg() << *block << ":\n";
  }




  // FUNCTION

  jit_function::jit_function(): local_count(0),
				m_shared(false),
				m_include_math_ptx_unary(PTX::map_ptx_math_functions_unary.size(),false),
				m_include_math_ptx_binary(PTX::map_ptx_math_functions_binary.size(),false)
  {
    entry_block = jit_ins_start_new_block();
  }


  void jit_function::emitShared() {
    m_shared=true;
  }


  int jit_function::reg_alloc( jit_llvm_type type ) {
    return reg_count[type]++;
  }

  std::ostringstream& jit_function::get_prg() { return oss_prg; }
  std::ostringstream& jit_function::get_signature() { return oss_signature; }





  void jit_function::write_reg_defs()
  {
    // for( int i = 0 ; i < vec_local_count.size() ; ++i ) 
    //   {
    // 	jit_llvm_type type = vec_local_count.at(i).first;
    // 	int count = vec_local_count.at(i).second;
    // 	oss_reg_defs << ".local ." 
    // 		     << jit_get_ptx_type( type ) << " " 
    // 		     << jit_get_identifier_local_memory() << i 
    // 		     << "[" << count << "];\n";
    //   }
  }


  std::string jit_function::get_kernel_as_string()
  {
    std::ostringstream final_ptx;
    write_reg_defs();

    int major = DeviceParams::Instance().getMajor();
    int minor = DeviceParams::Instance().getMinor();
    
    if (major >= 2) {
      final_ptx << ".version 3.1\n";
      final_ptx << ".target sm_" << major << minor << "\n";
      final_ptx << ".address_size 64\n";
    } else {
      final_ptx << ".version 1.4\n";
      final_ptx << ".target sm_" << major << minor << "\n";
    }

    if (m_shared)
      final_ptx << ".extern .shared .align 4 .b8 sdata[];\n";

    for( int i=0 ; i < PTX::map_ptx_math_functions_unary.size() ; i++ ) {
      if (m_include_math_ptx_unary.at(i)) {
	QDP_info_primary("including unary PTX math function %d",(int)i);
	final_ptx << jit_get_map_ptx_math_functions_prg_unary(i) << "\n";
      }
    }
    for( int i=0 ; i < PTX::map_ptx_math_functions_binary.size() ; i++ ) {
      if (m_include_math_ptx_binary.at(i)) {
	QDP_info_primary("including binary PTX math function %i",(int)i);
	final_ptx << jit_get_map_ptx_math_functions_prg_binary(i) << "\n";
      }
    }

    final_ptx << ".entry function (" 
	<< get_signature().str() 
	<< ")\n" 
	<< "{\n" 
	<< oss_reg_defs.str() 
	<< oss_prg.str() 
	<< "}\n";

    return final_ptx.str();
  }



  jit_value_t jit_add_param( jit_llvm_type type ) {

    jit_function_t func = jit_get_function();

    static bool first_param = true;
    if (!first_param)
      func->get_signature() << ",\n";
    first_param = false;

    jit_value_t param(type);
    func->get_signature() << param->get_type() << " "
			  << param << " ";
    param->set_ever_assigned();
    return param;
  }


  // int jit_function::local_alloc( jit_llvm_type type, int count ) {
  //   assert(count>0);
  //   int ret =  vec_local_count.size();
  //   vec_local_count.push_back( std::make_pair(type,count) );
  //   return ret;
  // }
  
  
  // jit_value_t jit_allocate_local( jit_llvm_type type , int count ) {
  //   jit_function_t func = jit_get_function();
  //   int num = func->local_alloc(type,count);
  //   jit_value_t ret( jit_llvm_type::u64 );
  //   func->get_prg() << "mov.u64 " 
  // 		    << jit_get_reg_name(ret) 
  // 		    << "," 
  // 		    << jit_get_identifier_local_memory() << num 
  // 		    << ";\n";
  //   ret.set_ever_assigned();
  //   // ret.set_state_space( jit_state_space::state_local );
  //   return ret;
  // }


  // jit_value_t jit_get_shared_mem_ptr( ) {
  //   jit_function_t func = jit_get_function();
  //   jit_value_t ret( jit_llvm_type::u64 );
  //   func->get_prg() << "mov.u64 " 
  // 		    << jit_get_reg_name(ret) 
  // 		    << ",sdata;\n";
  //   ret.set_ever_assigned();
  //   // ret.set_state_space( jit_state_space::state_shared );
  //   func->emitShared();
  //   return ret;
  // }


  std::string jit_get_kernel_as_string() {
    std::string ret = jit_get_function()->get_kernel_as_string();
    QDP_info_primary("Resetting jit function");
    jit_internal_function.reset();
    return ret;
  }


  void jit_start_new_function() {
    if (jit_internal_function) {
      QDP_error_exit("New jit function requested, but previous one not finished yet!");
      //QDP_info_primary("Resetting old jit function (use_count = %d) ...",(int)jit_internal_function.use_count());
      //jit_internal_function.reset();
    }
    QDP_info_primary("Starting new jit function");
    jit_internal_function = make_shared<jit_function>();
  }


  jit_function_t jit_get_function() {
    assert( jit_internal_function );
    return jit_internal_function;
  }


  jit_block_t jit_get_entry_block() {
    return jit_get_function()->get_entry_block();
  }


  CUfunction jit_get_cufunction(const char* fname)
  {
    CUfunction func;
    CUresult ret;
    CUmodule cuModule;

    std::string ptx_kernel = jit_get_kernel_as_string();

#if 1
    // Write kernel to file ?
    if (Layout::primaryNode()) {
      std::ofstream out(fname);
      out << ptx_kernel;
      out.close();
    }
#endif

    QDP_error_exit("End");

    ret = cuModuleLoadDataEx( &cuModule , ptx_kernel.c_str() , 0 , 0 , 0 );
    if (ret) {
      if (Layout::primaryNode()) {
	QDP_info_primary("Error loading external data. Dumping kernel to %s.",fname);
	std::ofstream out(fname);
	out << ptx_kernel;
	out.close();
	QDP_error_exit("Abort.");
      }
    }

    ret = cuModuleGetFunction(&func, cuModule, "function");
    if (ret)
      QDP_error_exit("Error returned from cuModuleGetFunction. Abort.");

    return func;
  }






  // VALUE REG


  jit_value::jit_value():   
    ever_assigned(false) 
  {
    reg_alloc(); 
  }

  jit_value::jit_value( const jit_value& rhs ):
    type(rhs.type), 
    mem_state(rhs.mem_state),
    ever_assigned(rhs.ever_assigned)
  {
    reg_alloc();
    assign( rhs );
  }
    
  void jit_value::reg_alloc() {
    //assert( type != jit_llvm_type::u8 );
    number = jit_get_function()->reg_alloc();
  }
  

  // void jit_value::assign( const jit_value& rhs ) {
  //   assert( !this->ever_assigned );
  //   assert( rhs.get_ever_assigned() );

  //   jit_get_function()->get_prg() << "mov."
  // 				  << jit_get_ptx_type( dest.get_type() ) << " "
  // 				  << jit_get_reg_name( dest ) << ","
  // 				  << jit_get_reg_name( src ) << ";\n";
  //   dest.set_state_space( src.get_state_space() );
  //   dest.set_ever_assigned();

  //   if ( type != rhs.get_type() ) {
  //     jit_ins_assign( *this , jit_val_convert( type , rhs ) );
  //   } else {
  //     jit_ins_mov( *this , rhs );
  //   }
  //   ever_assigned = rhs.ever_assigned;
  // }


  // jit_value& jit_value::operator=( const jit_value& rhs ) {
  //   assign(rhs);
  // }
    


  jit_llvm_type    jit_value::get_type() const {return type;}

  // void            jit_value_t::set_state_space( jit_state_space ss ) { mem_state = ss; }
  // jit_state_space jit_value_t::get_state_space() const { 
  //   return mem_state; 
  // }



  void jit_ins_bar_sync( int a ) {
    jit_function_t func = jit_get_function();
    assert( a >= 0 && a <= 15 );
    func->get_prg() << "bar.sync " << a << ";\n";
  }

  
  
  
  // int jit_value_t::get_number() const { return number; }
  
  // jit_function_t jit_value_t_reg::get_func() const { return func; };






    
  // jit_value_t_reg_t jit_val_create_new( int type ) {
  //   jit_function_t func = jit_get_function();
  //   //std::cout << "Creating jit value, type = " << type << "\n";
  //   jit_value_t_reg_t val( new jit_value_t_reg( type ) );
  //   return val;
  // }

  // jit_value_t_reg_t jit_val_create_from_const( int type , int val_i , const jit_value_t& pred) {
  //   jit_function_t func = jit_get_function();
  //   //std::cout << "Creating const jit value, type = " << type << "\n";
  //   jit_value_t_const_t val_const(new jit_value_t_const_int(val_i));
  //   return jit_val_create_convert( type , val_const , pred );
  // }


  jit_value_t jit_val_convert( jit_llvm_type type , const jit_value_t& rhs ) {
    assert(rhs);
    assert( type.get_ind() == rhs->get_type().get_ind() );
    assert( type.get_ind() == jit_llvm_builtin::no );

    if (rhs->get_type() == type) {
      return rhs;
    } else {

      jit_value_t ret = create_jit_value(type);

      //  %mul_i1 = sitofp i1 %param_i1 to double
      jit_get_function()->get_prg() << ret << " = "
				    << jit_get_map_cvt_from_to( rhs->get_type().get_builtin() , type.get_builtin() ) << " "
				    << rhs->get_type().get_builtin() << " " 
				    << rhs << " to "
				    << type.get_builtin() << "\n";
      ret.set_ever_assigned();
      return ret;
    }
  }


  // jit_value_t_reg_t jit_val_create_convert_const( int type , jit_value_t_const_t val , const jit_value_t& pred ) {
  //   jit_function_t func = jit_get_function();
  //   jit_value_t_reg_t ret = jit_val_create_new( type );
  //   assert( type != jit_llvm_type::u8 );
  //   func->get_prg() << jit_predicate(pred)
  // 		    << "mov." 
  // 		    << jit_get_ptx_type( type ) << " " 
  // 		    << ret->get_name() << ","
  // 		    << val->getAsString() << ";\n";
  //   return ret;
  // }
  // jit_value_t_reg_t jit_val_create_convert_reg( int type , jit_value_t_reg_t val , const jit_value_t& pred ) {
  //   jit_function_t func = jit_get_function();
  //   jit_value_t_reg_t ret = jit_val_create_new( type );
  //   if (type == val.get_type()) {
  //     assert( type != jit_llvm_type::u8 );
  //     func->get_prg() << jit_predicate(pred)
  // 		      << "mov." 
  // 		      << jit_get_ptx_type( type ) << " " 
  // 		      << ret->get_name() << ","
  // 		      << val->get_name() << ";\n";
  //   } else {
  //     if ( type == jit_llvm_type::pred ) {
  // 	ret = get<jit_value_t_reg>(jit_ins_ne( val , jit_value_t(0) , pred ));
  //     } else if ( val.get_type() == jit_llvm_type::pred ) {
  // 	jit_value_t ret_s32 = jit_ins_selp( jit_value_t(1) , jit_value_t(0) , val );
  // 	if (type != jit_llvm_type::s32)
  // 	  return jit_val_create_convert( jit_llvm_type::s32 , ret_s32 , pred );
  // 	else
  // 	  return get<jit_value_t_reg>(ret_s32);
  //     } else {
  // 	func->get_prg() << jit_predicate(pred)
  // 			<< "cvt."
  // 			<< jit_get_map_cvt_rnd_from_to(val.get_type(),type) 
  // 			<< jit_get_ptx_type( type ) << "." 
  // 			<< jit_get_ptx_type( val.get_type() ) << " " 
  // 			<< ret->get_name() << ","
  // 			<< val->get_name() << ";\n";
  //     }
  //   }
  //   return ret;
  // }
  // jit_value_t_reg_t jit_val_create_convert( int type , jit_value_t val , const jit_value_t& pred ) {
  //   assert(val);
  //   if (auto val_const = get< jit_value_t_const >(val))
  //     return jit_val_create_convert_const( type , val_const , pred );
  //   if (auto val_reg = get< jit_value_t_reg >(val))
  //     return jit_val_create_convert_reg( type , val_reg , pred );
  //   assert(!"Probs");
  // }

  // jit_value_t jit_val_create_copy( jit_value_t val , const jit_value_t& pred ) {
  //   assert(val);
  //   if (auto val_const = get< jit_value_t_const >(val)) 
  //     {
  // 	if (jit_value_t_const_int_t val_const_int = get<jit_value_t_const_int>(val_const)) 
  // 	  {
  // 	    return jit_value_t( val_const_int->getValue()  );
  // 	  } 
  // 	if (jit_value_t_const_float_t val_const_float = get<jit_value_t_const_float>(val_const)) 
  // 	  {
  // 	    return jit_val_create_const_float( val_const_float->getValue()  );
  // 	  }
  // 	assert(!"Problem");
  //     }
  //   if (auto val_reg = get< jit_value_t_reg >(val))
  //     {
  // 	// std::cout << "TYPE reg = " << val_reg.get_type() << "\n";
  // 	// std::cout << "TYPE     = " << val.get_type() << "\n";
  // 	assert( val_reg.get_type() != jit_llvm_type::u8 );
  // 	jit_value_t_reg_t ret = jit_val_create_convert( val_reg->get_func() , val_reg.get_type() , val_reg , pred );
  // 	return ret;
  //     }
  //   assert(!"Problem");
  // }


  // jit_value_t_const_t jit_value_t( int val ) {
  //   return std::make_shared< jit_value_t_const_int >(val);
  // }

  // jit_value_t_const_t jit_val_create_const_float( double val ) {
  //   return std::make_shared< jit_value_t_const_float >(val);
  // }




  // Thread Geometry

  jit_value_t jit_geom_get_special( const char* reg_name ) {
    jit_value_t ret = create_jit_value( jit_llvm_builtin::i32 );
    jit_get_function()->get_prg() << ret << " = call "
				  << ret->get_type() << " " 
				  << reg_name << "\n";
    ret.set_ever_assigned();
    return ret;
  }

  jit_value_t jit_geom_get_tidx()  { return jit_geom_get_special("@llvm.nvvm.read.ptx.sreg.tid.x()"); }
  jit_value_t jit_geom_get_ntidx() { return jit_geom_get_special("@llvm.nvvm.read.ptx.sreg.ntid.x()"); }
  jit_value_t jit_geom_get_ctaidx(){ return jit_geom_get_special("@llvm.nvvm.read.ptx.sreg.ctaid.x()"); }




  // jit_value_t jit_ins_selp( const jit_value_t& lhs ,  const jit_value_t& rhs , const jit_value_t& p ) {
  //   jit_llvm_type typebase = jit_type_promote( lhs.get_type() , rhs.get_type() );
  //   jit_value_t ret = create_jit_value( typebase );
  //   std::ostringstream instr;
    
  //   if (typebase == jit_llvm_type::pred) {
  //     assert( lhs.get_type() == jit_llvm_type::pred );
  //     assert( rhs.get_type() == jit_llvm_type::pred );
  //     typebase = jit_llvm_type::s32;
  //     jit_value_t lhs_s32 = create_jit_value(typebase);
  //     jit_value_t rhs_s32 = create_jit_value(typebase);
  //     jit_value_t ret_s32 = create_jit_value(typebase);
  //     lhs_s32 = jit_ins_selp( jit_value_t(1) , jit_value_t(0) , lhs );
  //     rhs_s32 = jit_ins_selp( jit_value_t(1) , jit_value_t(0) , rhs );
  //     instr << "selp." 
  // 	    << jit_get_ptx_type( typebase ) 
  // 	    << " "
  // 	    << jit_get_reg_name( ret_s32 ) 
  // 	    << ","
  // 	    << jit_get_reg_name( lhs_s32 ) 
  // 	    << ","
  // 	    << jit_get_reg_name( rhs_s32 ) 
  // 	    << ","
  // 	    << jit_get_reg_name( p ) 
  // 	    << ";\n";
  //     jit_get_function()->get_prg() << instr.str();
  //     ret = jit_ins_ne( ret_s32 , jit_value_t(0) );
  //     // ret.set_state_space( jit_state_promote( lhs.get_state_space() , rhs.get_state_space() ) );
  //     ret.set_ever_assigned();
  //     return ret;
  //   }

  //   jit_value_t lhs_tb = create_jit_value(typebase);
  //   jit_value_t rhs_tb = create_jit_value(typebase);

  //   lhs_tb = lhs.get_type() != typebase ? jit_val_convert( typebase , lhs ) : lhs;
  //   rhs_tb = rhs.get_type() != typebase ? jit_val_convert( typebase , rhs ) : rhs;

  //   instr << "selp." 
  // 	  << jit_get_ptx_type( typebase ) 
  // 	  << " "
  // 	  << jit_get_reg_name( ret ) 
  // 	  << ","
  // 	  << jit_get_reg_name( lhs_tb ) 
  // 	  << ","
  // 	  << jit_get_reg_name( rhs_tb ) 
  // 	  << ","
  // 	  << jit_get_reg_name( p ) 
  // 	  << ";\n";

  //   jit_get_function()->get_prg() << instr.str();
  //   // ret.set_state_space( jit_state_promote( lhs.get_state_space() , rhs.get_state_space() ) );
  //   ret.set_ever_assigned();
  //   return ret;
  // }





  jit_value_t jit_ins_op( const jit_value_t& lhs , const jit_value_t& rhs , const JitOp& op ) {
    jit_llvm_type dest_type = op.getDestType();
    jit_llvm_type args_type = op.getArgsType();
    jit_value_t ret = create_jit_value( dest_type );
    jit_value_t lhs_new = jit_val_convert( args_type , lhs );
    jit_value_t rhs_new = jit_val_convert( args_type , rhs );
    jit_get_function()->get_prg() << ret << " = "
				  << op << " "
				  << lhs_new << ","
				  << rhs_new << ";\n";
    ret.set_ever_assigned();
    //// ret.set_state_space( jit_state_promote( lhs.get_state_space() , rhs.get_state_space() ) );
    return ret;
  }



  jit_value_t jit_ins_add( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpAdd( lhs , rhs ) );
  }
  jit_value_t jit_ins_sub( const jit_value_t& lhs , const jit_value_t& rhs) {
    return jit_ins_op( lhs , rhs , JitOpSub( lhs , rhs ) );
  }
  jit_value_t jit_ins_mul( const jit_value_t& lhs , const jit_value_t& rhs) {
    return jit_ins_op( lhs , rhs , JitOpMul( lhs , rhs ) );
  }
  jit_value_t jit_ins_div( const jit_value_t& lhs , const jit_value_t& rhs) {
    return jit_ins_op( lhs , rhs , JitOpDiv( lhs , rhs ) );
  }
  jit_value_t jit_ins_shl( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpSHL( lhs , rhs ) );
  }
  jit_value_t jit_ins_shr( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpSHR( lhs , rhs ) );
  }
  jit_value_t jit_ins_mul_wide( const jit_value_t& lhs , const jit_value_t& rhs) {
    return jit_ins_op( lhs , rhs , JitOpMulWide( lhs , rhs ) );
  }
  jit_value_t jit_ins_and( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpAnd( lhs , rhs ) );
  }
  jit_value_t jit_ins_or( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpOr( lhs , rhs ) );
  }
  jit_value_t jit_ins_xor( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpXOr( lhs , rhs ) );
  }
  jit_value_t jit_ins_rem( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpRem( lhs , rhs ) );
  }

  jit_value_t jit_ins_lt( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpLT( lhs , rhs ) );
  }
  jit_value_t jit_ins_ne( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpNE( lhs , rhs ) );
  }
  jit_value_t jit_ins_eq( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpEQ( lhs , rhs ) );
  }
  jit_value_t jit_ins_ge( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpGE( lhs , rhs ) );
  }
  jit_value_t jit_ins_le( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpLE( lhs , rhs ) );
  }
  jit_value_t jit_ins_gt( const jit_value_t& lhs , const jit_value_t& rhs ) {
    return jit_ins_op( lhs , rhs , JitOpGT( lhs , rhs ) );
  }


  jit_value_t jit_ins_or( const jit_value_t& lhs , const jit_value_t& rhs ) { assert(!"ni"); }
  jit_value_t jit_ins_and( const jit_value_t& lhs , const jit_value_t& rhs ) { assert(!"ni"); }
  jit_value_t jit_ins_xor( const jit_value_t& lhs , const jit_value_t& rhs ) { assert(!"ni"); }
  jit_value_t jit_ins_mod( const jit_value_t& lhs , const jit_value_t& rhs ) { assert(!"ni"); }




  jit_value_t jit_ins_unary_op( const jit_value_t& reg , const JitUnaryOp& op ) {
    jit_llvm_type type = reg->get_type();
    jit_value_t ret = create_jit_value( type );
    jit_get_function()->get_prg() << ret << " = "
				  << op << " "
				  << reg << ";\n";
    ret.set_ever_assigned();
    // ret.set_state_space( ret.get_state_space() );
    return ret;
  }


  jit_value_t jit_ins_neg( const jit_value_t& rhs ) {
    return jit_ins_unary_op( rhs , JitUnaryOpNeg( rhs.get_type() ) );
  }
  jit_value_t jit_ins_not( const jit_value_t& rhs ) {
    return jit_ins_unary_op( rhs , JitUnaryOpNot( rhs.get_type() ) );
  }
  jit_value_t jit_ins_fabs( const jit_value_t& rhs ) {
    return jit_ins_unary_op( rhs , JitUnaryOpAbs( rhs.get_type() ) );
  }
  jit_value_t jit_ins_floor( const jit_value_t& rhs ) {
    return jit_ins_unary_op( rhs , JitUnaryOpFloor( rhs.get_type() ) );
  }
  jit_value_t jit_ins_ceil( const jit_value_t& rhs ) {
    return jit_ins_unary_op( rhs , JitUnaryOpCeil( rhs.get_type() ) );
  }
  jit_value_t jit_ins_sqrt( const jit_value_t& rhs ) { 
    return jit_ins_unary_op( rhs , JitUnaryOpSqrt( rhs.get_type() ) );
  }


  //
  //   %arrayidx = getelementptr i32* %data, i64 %idxprom
  //
  jit_value_t jit_ins_getelementptr( const jit_value_t& base , const jit_value_t& offset ) {
    assert( base->get_type().ind == jit_llvm_ind::yes );
    jit_value_t ret = create_jit_value( base->get_type() );
    jit_get_function()->get_prg() << ret
				  << " = getelementptr " 
				  << ret->get_type() << " "
				  << base << ", "
				  << offset->get_type() << " "
				  << offset << "\n";
    return ret;
  }

  //
  //  %val = load i32* %ptr                           ; yields {i32}:val = i32 3
  //
  jit_value_t jit_ins_load( const jit_value_t& base , const jit_value_t& offset , jit_llvm_type type ) {
    assert( base->get_type().ind == jit_llvm_ind::yes );
    assert( base->get_type() == type );
    jit_value_t ptr = jit_ins_getelementptr(base,offset);
    jit_value_t ret = create_jit_value( type );
    jit_get_function()->get_prg() << ret
				  << " = load "
				  << ptr->get_type() << " "
				  << ptr << "\n";
    return ret;
  }

  //
  //  store i32 %call, i32* %arrayidx, align 4
  //
  void jit_ins_store(const jit_value_t& base, const jit_value_t& offset, jit_llvm_type type, const jit_value_t& reg ) {
    assert( base->get_type().ind == jit_llvm_ind::yes );
    assert( base->get_type() == type );
    jit_value_t ptr = jit_ins_getelementptr(base,offset);
    jit_get_function()->get_prg() << "store "
				  << type << " "
				  << reg << ", "
				  << ptr->get_type() << " "
				  << ptr << "\n";
  }




  jit_value_t jit_geom_get_linear_th_idx() {
    jit_value_t ctaidx = jit_geom_get_ctaidx();
    jit_value_t ntidx  = jit_geom_get_ntidx();
    return jit_ins_add( jit_ins_mul( ctaidx , ntidx ) , jit_geom_get_tidx() );
  }


  // std::string jit_predicate( const jit_value_t& pred ) {
  //   if (!pred.get_ever_assigned())
  //     return "";
  //   assert( pred.get_type() == jit_llvm_type::pred );
  //   std::ostringstream oss;
  //   oss << "@" << jit_get_reg_name(pred) << " ";
  //   return oss.str();
  // }



  void jit_ins_exit() {
    jit_get_function()->get_prg() << "ret void\n";
  }



  void jit_ins_branch( const jit_value_t& cond,  jit_block_t& block_true , jit_block_t& block_false ) {
    assert(cond);
    if (!block_true)
      block_true = jit_block_create();
    if (!block_false)
      block_false = jit_block_create();
    jit_get_function()->get_prg() << "br "
				  << cond->get_type() << " "
				  << cond << ", "
				  << "label " << *block_true << ", ";
				  << "label " << *block_false << "\n";
  }

  void jit_ins_branch( jit_block_t& block ) {
    if (!block)
      block = jit_block_create();
    jit_get_function()->get_prg() << "br "
				  << "label " << *block_false << "\n";
  }

  void jit_ins_comment( const char * comment ) {
    jit_get_function()->get_prg() << "; " << comment << "\n";
  }

}



