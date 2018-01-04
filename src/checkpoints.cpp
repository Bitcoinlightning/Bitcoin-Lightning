// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		(0, uint256("0x0000153d1dea38532ed6798bc1bb0092b41d0b487faeafcace26ab8375e93475"))
		(24, uint256("0xfb0553457c040945d758ccd6305906d7b1097ef27ba01db6b1c5901af1389b88"))
		(80, uint256("0x61e84eaaef553de5eac76772ab470bf6f7a42e2f9153fb9885b9267425076228"))
		(782, uint256("0x7aec63f150f57b0eb69eef21e497d7acc41a951068a47da8996afde8791c6fa4"))
		(2584, uint256("0x2bdae496ebc5697d1668c9e6a5446b93bd3b345c9eb2294d145658aafd5e8c37"))
		(3951, uint256("0x10cc8881db338a82cdc450141bff6e4a4dd9ca6a109b3ee57332348a4fa1526b"))
		(6667, uint256("0xa30519ba5a30fe216528160e60c4063eb552aa8bcb241c4276ff0bb37864431d"))
		(9428, uint256("0x2a6121a8d0dda032eb88f7a35d626fb08c07a94cefc515b0174b00e9378d7d62"))
		(10543, uint256("0xf31e7b9e80bc7b8da00b0f1d65e9cffa76e73b5dec32d6b8c2a2cd3841851ebb"))
		(12598, uint256("0x9e9c98054b890f5e0704ef7ce00c6050adea75946b8c65e13be9dc0d0993074c"))
		(14726, uint256("0x34ad659bf5ad581af3e9c9c5ea1aa9ee360ce6874a92712a64971f2cb4666bb7"))
		(16198, uint256("0x53ea1d24e6c118fbc5aef6a5a2012370455669b59e5a14e52f7ed11258ca112c"))
		
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
