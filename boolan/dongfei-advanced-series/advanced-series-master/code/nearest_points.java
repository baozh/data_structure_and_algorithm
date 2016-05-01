// Given a number of points on a plane. Implement a method that by given center 
// point and number P would return P nearest to the center points.

class PointsOnAPlaneSolution implements  PointsOnAPlane {
 
    private final List<Point> points = new ArrayList<Point>();
 
    public void addPoint(Point point) {
        points.add(point);
    }
 
    public Collection<Point> findNearest(Point center, int number) {
        Comparator<Point> comparator = comparatorByDistance(center);
        int limit = number + 1;
        PriorityQueue<Point> queue = newPriorityQueue<Point>(limit, comparator);
        for(Point p: points) {
            queue.offer(p);
            if(queue.size() == limit) {
                queue.poll();
            }
        }
        return queue;
    }
 
    private Comparator<Point> comparatorByDistance(finalPoint center) {
        return newComparator<Point>() {
                public int compare(Point point1, Point point2) {
                    Integer distSquare1 = distSquare(center, point1);
                    Integer distSquare2 = distSquare(center, point2);
                    return distSquare2.compareTo(distSquare1);
                }
            };
    }
 
    public static int distSquare(Point a, Point b) {
        intdx = a.x - b.x;
        intdy = a.y - b.y;
        return dx*dx + dy*dy;
    }
}
