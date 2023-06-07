// segment.h


class Segment {
 public:

  vec3 tail, head;

  Segment( vec3 t, vec3 h )
    { tail = t; head = h; }

  bool intersects( Segment const& s2 ) {

    // YOUR CODE HERE
      bool intersect = false;

      intersect = turnDir(head, s2.head, s2.tail) != turnDir(tail, s2.head, s2.tail) && 
          turnDir(head, tail, s2.head) != turnDir(head, tail, s2.tail);

      return intersect;
  }


  int turnDir(vec3 p1, vec3 p2, vec3 p3) {
      
      float turn;
      int val;
      int LEFT_TURN = 0;
      int RIGHT_TURN = 1;
      int COLLINEAR = 2;
      
      // compute orientation of 3 points 
      // turn can take on 3 values: 
      // if turn is positive then the points create a CCW turn(left)
      // if turn is negative then the points create a CW turn(right)
      // otherwise the points are collinear(2)
      turn = (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);

      if (turn > 0) val = LEFT_TURN;
      else if (turn < 0) val = RIGHT_TURN;
      else val = COLLINEAR;

      return val;
  }

};


