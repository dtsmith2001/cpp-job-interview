//
// Created by Dale Smith on 1/19/17.
//

#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <boost/tokenizer.hpp>

//
// build with build.sh instead of a makefile (no time for that)
//
// Cut the columns required out of the big file using
// cut -f1,13 -d',' DE1_0_2008_to_2010_Carrier_Claims_Sample_1A.csv > columns.csv
// preprocessing saves time
//
// Get unique providers (498,018: tail -n +2 columns.csv | cut -f2 -d',' | sort -u | wc -l
// Get unique beneficiaries (49,278): tail -n +2 columns.csv | cut -f1 -d',' | sort -u | wc -l
//
// Validate claim count with
// grep 0000313693 columns.csv | wc -l
// and beneficiary count with
// grep 0000313693 columns.csv | cut -f1 -d',' | sort -u | wc -l
// see the script validate.sh
// validate.sh <provider>
//
// 0000313693,7,4
// 9999786931,4,3
// 0005508410,17,14
// 0000434979,15,9
// 0000458267,14,14
// 0000869846,1,1
// 0011809664,38,23
// 0027420316,30,11
// 9970511858,5,5
// 9965671342,15,12
// 9961583823,11,9
//

int main(void) {
    using namespace std;
    using namespace boost;
    // note - reading the actual file is much slower than pre-processing the data with cut (see above)
//    ifstream file("DE1_0_2008_to_2010_Carrier_Claims_Sample_1A.csv");
    ifstream file("columns.csv");
    if (!file.is_open()) {
        cout << "file not found" << endl;
        return 1;
    }

    // typedefs make things look neater
    typedef pair<size_t, size_t> pair_v;
    typedef pair<string, string> pair_set_k;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer; // http://stackoverflow.com/a/2322735

    vector<string> vec(2); // number of columns 142: head -1 DE1_0_2008_to_2010_Carrier_Claims_Sample_1A.csv | awk -F, '{print NF}'
    string line;

    // provider,n_claims,n_beneficiaries
    map<string, pair_v> providers;

    // unique provider,beneficiary
    set<pair_set_k> beneficiaries;

    string beneficiary;
    string provider;

    getline(file, line); // skip header
    while(getline(file, line)) { // this processes only the data
        // Parsing a file is a known thing - got some code from
        // http://stackoverflow.com/a/2322735
        Tokenizer tok(line);
        vec.assign(tok.begin(), tok.end());
        beneficiary = vec[0];
        provider = vec[1];
        vec.clear();
        // have we seen the provider,beneficiary pair yet?
        auto search = beneficiaries.find( make_pair(provider, beneficiary) );
        if (search == beneficiaries.end()) {
            // no, we haven't seen the provider,beneficiary pair - insert
            beneficiaries.insert( make_pair(provider, beneficiary) );
            // search for the provider
            auto prov_search = providers.find(provider);
            if (prov_search == providers.end()) {
                // we didn't find an entry so insert the provider, recording a claim and a new beneficiary
                providers.insert( make_pair(provider, make_pair(1, 1)) );
            } else {
                // we did find the provider - increment claims count
                providers[provider].first++;
                // we have a new beneficiary - increment beneficiary count
                providers[provider].second++;
            }
        } else {
            // we have seen the provider-beneficiary pair before, so it's in the providers map
            // update the claims; do not update the beneficiaries since we want unique beneficiaries
            providers[provider].first++;
        }
    }

    cout << "PRF_PHYSN_NPI_1,n_claims,n_beneficiaries" << endl;
    for (const auto& i: providers) {
        cout << i.first << "," << i.second.first << "," << i.second.second << endl;
    }
}
