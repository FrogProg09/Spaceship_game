// class for identifying positions of different things on the screen
class point_2d{
	public:
    point_2d(int value_x, int value_y) : x(value_x), y(value_y) {}

		int x;
		int y;

    point_2d operator+ (const point_2d b) const{
      return point_2d (x + b.x, y + b.y); 
    }

    point_2d operator- (const point_2d b) const{
      return point_2d (x - b.x, y - b.y); 
    }
};
