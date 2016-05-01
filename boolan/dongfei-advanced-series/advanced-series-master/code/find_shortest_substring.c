// given a string words, find the shortest substring including all the given key words


// Returns false if no valid window is found. Else returns
// true and updates minWindowBegin and minWindowEnd with the
// starting and ending position of the minimum window.
bool minWindow(constchar* S, constchar*T,
               int&minWindowBegin, int&minWindowEnd) {
  intsLen = strlen(S);
  inttLen = strlen(T);
  intneedToFind[256] = {0};
 
  for(inti = 0; i < tLen; i++)
    needToFind[T[i]]++;
 
  inthasFound[256] = {0};
  intminWindowLen = INT_MAX;
  intcount = 0;
  for(intbegin = 0, end = 0; end < sLen; end++) {
    // skip characters not in T
    if(needToFind[S[end]] == 0) continue;
    hasFound[S[end]]++;
    if(hasFound[S[end]] <= needToFind[S[end]])
      count++;
 
    // if window constraint is satisfied
    if(count == tLen) {
      // advance begin index as far right as possible,
      // stop when advancing breaks window constraint.
      while(needToFind[S[begin]] == 0||
            hasFound[S[begin]] > needToFind[S[begin]]) {
        if(hasFound[S[begin]] > needToFind[S[begin]])
          hasFound[S[begin]]--;
        begin++;
      }
 
      // update minWindow if a minimum length is met
      intwindowLen = end - begin + 1;
      if(windowLen < minWindowLen) {
        minWindowBegin = begin;
        minWindowEnd = end;
        minWindowLen = windowLen;
      } // end if
    } // end if
  } // end for
 
  return(count == tLen) ? true: false;
}
