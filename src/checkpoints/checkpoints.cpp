// Copyright (c) 2014-2018, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "include_base_utils.h"

using namespace epee;

#include "checkpoints.h"

#include "common/dns_utils.h"
#include "include_base_utils.h"
#include "string_tools.h"
#include "storages/portable_storage_template_helper.h" // epee json include
#include "serialization/keyvalue_serialization.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "checkpoints"

namespace cryptonote
{
  /**
   * @brief struct for loading a checkpoint from json
   */
  struct t_hashline
  {
    uint64_t height; //!< the height of the checkpoint
    std::string hash; //!< the hash for the checkpoint
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(height)
          KV_SERIALIZE(hash)
        END_KV_SERIALIZE_MAP()
  };

  /**
   * @brief struct for loading many checkpoints from json
   */
  struct t_hash_json {
    std::vector<t_hashline> hashlines; //!< the checkpoint lines from the file
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(hashlines)
        END_KV_SERIALIZE_MAP()
  };

  //---------------------------------------------------------------------------
  checkpoints::checkpoints()
  {
  }
  //---------------------------------------------------------------------------
  bool checkpoints::add_checkpoint(uint64_t height, const std::string& hash_str)
  {
    crypto::hash h = crypto::null_hash;
    bool r = epee::string_tools::parse_tpod_from_hex_string(hash_str, h);
    CHECK_AND_ASSERT_MES(r, false, "Failed to parse checkpoint hash string into binary representation!");

    // return false if adding at a height we already have AND the hash is different
    if (m_points.count(height))
    {
      CHECK_AND_ASSERT_MES(h == m_points[height], false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
    }
    m_points[height] = h;
    return true;
  }
  //---------------------------------------------------------------------------
  bool checkpoints::is_in_checkpoint_zone(uint64_t height) const
  {
    return !m_points.empty() && (height <= (--m_points.end())->first);
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h, bool& is_a_checkpoint) const
  {
    auto it = m_points.find(height);
    is_a_checkpoint = it != m_points.end();
    if(!is_a_checkpoint)
      return true;

    if(it->second == h)
    {
      MINFO("CHECKPOINT PASSED FOR HEIGHT " << height << " " << h);
      return true;
    }else
    {
      MWARNING("CHECKPOINT FAILED FOR HEIGHT " << height << ". EXPECTED HASH: " << it->second << ", FETCHED HASH: " << h);
      return false;
    }
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h) const
  {
    bool ignored;
    return check_block(height, h, ignored);
  }
  //---------------------------------------------------------------------------
  //FIXME: is this the desired behavior?
  bool checkpoints::is_alternative_block_allowed(uint64_t blockchain_height, uint64_t block_height) const
  {
    if (0 == block_height)
      return false;

    auto it = m_points.upper_bound(blockchain_height);
    // Is blockchain_height before the first checkpoint?
    if (it == m_points.begin())
      return true;

    --it;
    uint64_t checkpoint_height = it->first;
    return checkpoint_height < block_height;
  }
  //---------------------------------------------------------------------------
  uint64_t checkpoints::get_max_height() const
  {
    std::map< uint64_t, crypto::hash >::const_iterator highest = 
        std::max_element( m_points.begin(), m_points.end(),
                         ( boost::bind(&std::map< uint64_t, crypto::hash >::value_type::first, _1) < 
                           boost::bind(&std::map< uint64_t, crypto::hash >::value_type::first, _2 ) ) );
    return highest->first;
  }
  //---------------------------------------------------------------------------
  const std::map<uint64_t, crypto::hash>& checkpoints::get_points() const
  {
    return m_points;
  }

  bool checkpoints::check_for_conflicts(const checkpoints& other) const
  {
    for (auto& pt : other.get_points())
    {
      if (m_points.count(pt.first))
      {
        CHECK_AND_ASSERT_MES(pt.second == m_points.at(pt.first), false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
      }
    }
    return true;
  }

  bool checkpoints::init_default_checkpoints(network_type nettype)
  {
    if (nettype == TESTNET)
    {
      ADD_CHECKPOINT(0,     "48ca7cd3c8de5b6a4d53d2861fbdaedca141553559f9be9520068053cda8430b");
      ADD_CHECKPOINT(1000000, "46b690b710a07ea051bc4a6b6842ac37be691089c0f7758cfeec4d5fc0b4a258");
      return true;
    }
    if (nettype == STAGENET)
    {
      ADD_CHECKPOINT(0,       "76ee3cc98646292206cd3e86f74d88b4dcc1d937088645e9b0cbca84b7ce74eb");
      ADD_CHECKPOINT(10000,   "1f8b0ce313f8b9ba9a46108bfd285c45ad7c2176871fd41c3a690d4830ce2fd5");
      return true;
    }
	  //Halykcoin checkpoints
	  ADD_CHECKPOINT(0,       "418015bb9ae982a1975da7d79277c2705727a56894ba0fb246adaabb1f4632e3");
	  ADD_CHECKPOINT(100,     "1a70fa12fd2d6c618ef9d27871fdaca8e4949a582dc127e4b832118ca633533a");
	  ADD_CHECKPOINT(500,     "2637218907bcfa3e912c6c7e2f6b17dfe11a0507e21ec1c869d7ab7c18dc8b9b");
	  ADD_CHECKPOINT(1000,    "ce7a0aa453631f6c3114829b2d046739b2b07e1485af94c9951fa4df444c6c2b");
	  ADD_CHECKPOINT(1500,    "3823ac5b0841a3536f0cea929c4632a33d720d25e0aa808e68ccf4757b2063e4");
	  ADD_CHECKPOINT(2000,    "61e945315e9b82ed6e71c3d2c2e8d571c2c19fde84730dba63216f8313ddc66d");
	  ADD_CHECKPOINT(3000,    "13cb8175a5e3b2e0e5a8a076d5c7f02083613e009c71e3180d34031c7f661fc4");
	  ADD_CHECKPOINT(4000,     "1485859c0a1683beb19d5e795ec84ef8a0c334a8cb118531a972e1312e42fbc7");
	  ADD_CHECKPOINT(5000,     "66d839126107d2dbdd360d0aa27e6b64ee9fc24e18e1dbf35edf916177511c25");
	  ADD_CHECKPOINT(6000,     "4526c7016efbfed525003ec284749132ad0e9ceeab26a60d0afc1f567a6e7587");
	  ADD_CHECKPOINT(7000,     "40c5e7a90f4350e1ad057cc495d072fff9c483d31b2a6e1660aa53fe1d06d817");
	  ADD_CHECKPOINT(8000,     "9287c710cde0cb34afb52615e99807e841a8b55020722595d357c6f1797b5b96");
	  ADD_CHECKPOINT(9000,     "52d17a11cee412079e78d31e6981871c60ac2e884256a70a4883b59ffd24567f");
	  ADD_CHECKPOINT(10000,     "c91976c7e90d1afb31865f6c5f5c1ffe238834458172bd53c0994796624548a3");
	  ADD_CHECKPOINT(30000,     "6c343a9f79493497cb544bf66e2453f00791dcb4b65ec43bcbb11b5c2cd7245b");
	  ADD_CHECKPOINT(40000,     "e76b861ab0e14d4bbcbd62784487523d2f3773900c55bca61db3a3115fd76ca8");
	  ADD_CHECKPOINT(50000,     "f1c192097ededb6752134190a0a838b7194092db6310e6f694742a1f6d5bc245");
	  ADD_CHECKPOINT(60000,     "8e0e0d2b8eaff15ed9a9455a03edfd89bce32af549014e280dec13009bfb2c03");
	  ADD_CHECKPOINT(70000,     "b27a131d6dd7f4f0530af085cbdf28f3222f97a4a0d0804c18e2113bf79c2bf2");
	  ADD_CHECKPOINT(80000,     "d59b17a90897c21c4e38ff3e73c8e4e4bb723949b0d6393a5cf66a6636bb4a77");
	  ADD_CHECKPOINT(90000,     "7b06fac92927322b81c9bbd10a33537f977e7f3686487ec7491fda17abb3b533");
	  ADD_CHECKPOINT(100000,     "f40141875b8bcd19e9223023d8821906d9870fd5d883f5ce1156d288f002657b");
	  ADD_CHECKPOINT(110000,     "738f90349fb94af94524e5519ab703017a79d4b8e82b133ebfede6c7bdb75b42");
	  ADD_CHECKPOINT(120000,     "2319844fef4235eb761f35f8dd87a754535973c62ccfdb073ee540be70cdf0c3");
	  ADD_CHECKPOINT(130000,     "a09008741323cd3cbaddcb63b3a13e9f72188fdd11b3486c51c3080c71312545");
	  ADD_CHECKPOINT(140000,     "25bbdb88cbb816a0112fa5db4162d6f83ee60fa9e3ce07c462e409a8d7d32ac3");

        //Tets talk about split
	  ADD_CHECKPOINT(275749,     "a78fdf312d6315bad43d2042d59abe7bebbae7ecc516dd152be3b43a9ba3fb9e");
	  ADD_CHECKPOINT(275750,     "43b418b9acd3f34770e4b18ec5b3f6f0abd9a19ebabb8ec2482e4d15013f479c");
	  ADD_CHECKPOINT(276481,     "a77aa1d17c6c95ccf39100c2ab001e1aef52e767e7060e4fcef2eade7a79b814");
      ADD_CHECKPOINT(276482,     "198588cf76a3b8a0d594720cf1c0ffe0cfed065374aff63f5846ce4334036c38");

    return true;
  }

  bool checkpoints::load_checkpoints_from_json(const std::string &json_hashfile_fullpath)
  {
    boost::system::error_code errcode;
    if (! (boost::filesystem::exists(json_hashfile_fullpath, errcode)))
    {
      LOG_PRINT_L1("Blockchain checkpoints file not found");
      return true;
    }

    LOG_PRINT_L1("Adding checkpoints from blockchain hashfile");

    uint64_t prev_max_height = get_max_height();
    LOG_PRINT_L1("Hard-coded max checkpoint height is " << prev_max_height);
    t_hash_json hashes;
    if (!epee::serialization::load_t_from_json_file(hashes, json_hashfile_fullpath))
    {
      MERROR("Error loading checkpoints from " << json_hashfile_fullpath);
      return false;
    }
    for (std::vector<t_hashline>::const_iterator it = hashes.hashlines.begin(); it != hashes.hashlines.end(); )
    {
      uint64_t height;
      height = it->height;
      if (height <= prev_max_height) {
	LOG_PRINT_L1("ignoring checkpoint height " << height);
      } else {
	std::string blockhash = it->hash;
	LOG_PRINT_L1("Adding checkpoint height " << height << ", hash=" << blockhash);
	ADD_CHECKPOINT(height, blockhash);
      }
      ++it;
    }

    return true;
  }

  bool checkpoints::load_checkpoints_from_dns(network_type nettype)
  {
    std::vector<std::string> records;

    // All four MoneroPulse domains have DNSSEC on and valid
    static const std::vector<std::string> dns_urls = { "checkpoints.halykcoin.org" 
    };

    static const std::vector<std::string> testnet_dns_urls = { "testpoints.halykcoin.org" 
    };

    static const std::vector<std::string> stagenet_dns_urls = { "stagenetpoints.moneropulse.se"
                   , "stagenetpoints.halykcoin.org"
                   , "stagenetpoints.halykcoin.net"
                   , "stagenetpoints.halykcoin.co"
    };

    if (!tools::dns_utils::load_txt_records_from_dns(records, nettype == TESTNET ? testnet_dns_urls : nettype == STAGENET ? stagenet_dns_urls : dns_urls))
      return true; // why true ?

    for (const auto& record : records)
    {
      auto pos = record.find(":");
      if (pos != std::string::npos)
      {
        uint64_t height;
        crypto::hash hash;

        // parse the first part as uint64_t,
        // if this fails move on to the next record
        std::stringstream ss(record.substr(0, pos));
        if (!(ss >> height))
        {
    continue;
        }

        // parse the second part as crypto::hash,
        // if this fails move on to the next record
        std::string hashStr = record.substr(pos + 1);
        if (!epee::string_tools::parse_tpod_from_hex_string(hashStr, hash))
        {
    continue;
        }

        ADD_CHECKPOINT(height, hashStr);
      }
    }
    return true;
  }

  bool checkpoints::load_new_checkpoints(const std::string &json_hashfile_fullpath, network_type nettype, bool dns)
  {
    bool result;

    result = load_checkpoints_from_json(json_hashfile_fullpath);
    if (dns)
    {
      result &= load_checkpoints_from_dns(nettype);
    }

    return result;
  }
}
