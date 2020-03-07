#pragma once
#define SAFE_DELETE(x) if(x!=nullptr) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY(x) if(x!=nullptr) { delete [] x; x = nullptr; }
#define SAFE_DELETE_VECTOR(x) while (!x.empty()) delete x.back(), x.pop_back();