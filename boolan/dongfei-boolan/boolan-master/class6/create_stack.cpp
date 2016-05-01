vector<Box> createStackDP( Box boxes[], const int num, Box bottom, unordered_map< Box, vector<Box> >& stackCache) {
    vector<Box> max_stack;
    int max_height = 0;
    vector<Box> new_stack;
  
    // memorization
    if( stackCache.count( bottom ) > 0 )
        return stackCache[ bottom ];
    else {
        for( int i = 0; i < num; i++ ) {
            if( Box[i].canBeAbove( bottom ) ) {
                // solve subproblem
                new_stack = createStackDP( boxes, num, Box[i], stackCache );
            }
            if( new_stack.size() > max_height ) {
                max_height = new_stack.size();
                max_stack = new_stack;
            }
        }
    }
  
    max_stack.insert( max_stack.begin(), bottom );
    stackCache[ bottom ] = max_stack;
    return max_stack;
}
