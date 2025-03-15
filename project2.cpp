#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
class Point {
public:
    int x, y;
    //Point constructor
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Rectangle {
public:
    float x_top_left, y_top_left, width, height; //variables for defining the rectangle.

    //Default Constructor
    Rectangle() {

    }
    //Parameterised constructor
    Rectangle(float x, float y, float width, float height) {
        this->x_top_left = x;
        this->y_top_left = y;
        this->width = width;
        this->height = height;
    }

    //For determining if the rectangle contains the point p.
    bool contains(Point p) {
        //checking which points should lie in the quardrant. 
    bool result = p.x >= x_top_left && p.x <= x_top_left + width && 
                  p.y <= y_top_left && p.y >= y_top_left - height;
    return result;
}

    //For determining if this rectangle intersects with other.    
    bool intersects(Rectangle other) {
        // return true if the main rectangle interesetcs with the other
        bool result = x_top_left < other.x_top_left + other.width &&
            x_top_left + width > other.x_top_left &&
            y_top_left > other.y_top_left - other.height &&
            y_top_left - height < other.y_top_left;

        return result;
    }
};

class twoDimSpace {
    Rectangle boundary;                     //Defines the boundary of this node
    int capacity;                       //Defines the number of points the node can hold.
    std::vector<Point> points;          //Storing the points in a templated vector (like Project 1)
    bool divided = false;               //A variable to store whether the node is further divided into quadrants
    twoDimSpace* northWest, * northEast; //Pointers to child nodes.
    twoDimSpace* southWest, * southEast; //Pointers to child nodes.

public:

    //Constructor for creating the node
    twoDimSpace(Rectangle boundary, int capacity) {
        this->boundary = boundary;
        this->capacity = capacity;
    }

    //Destructor
    ~twoDimSpace() {
        if (divided) {
            delete northWest;
            delete northEast;
            delete southWest;
            delete southEast;
        }
    }

    //Method to divide the boundary of this node into four equal quadrants.
    //The four quadrants will be used to instantiate the four children mentioned.
    void subdivide() {

        //Northwest rectangle boundaries defining
        Rectangle nwRect(boundary.x_top_left, boundary.y_top_left, boundary.width / 2, boundary.height / 2);
        northWest = new twoDimSpace(nwRect, capacity);//northwest object to store the points

        //NorthEast rectangle
        Rectangle neRect(boundary.x_top_left + boundary.width / 2, boundary.y_top_left, boundary.width / 2, boundary.height / 2);
        northEast = new twoDimSpace(neRect, capacity);

        //Southwest rectangle(we need to consider y_top_left-height because we need to calculate bottom right)
        Rectangle swRect(boundary.x_top_left, boundary.y_top_left - boundary.height / 2, boundary.width / 2, boundary.height / 2);
        southWest = new twoDimSpace(swRect, capacity);

        //Southeast rectangle(we need to consider y_top_left-height because we need to calculate bottom left)
        Rectangle seRect(boundary.x_top_left + boundary.width / 2, boundary.y_top_left - boundary.height / 2, boundary.width / 2, boundary.height / 2);
        southEast = new twoDimSpace(seRect, capacity);

        divided = true;

    }

    //Method for inserting a point in the node. If the node is full, it's children
    //are to be checked recursively.
    bool insert(Point point) {
        //within boundary
        if (!boundary.contains(point)) {
            return false;
        }
        //within capacity
        if (points.size() < capacity) {
            points.push_back(point);
            return true;
        }
        //if Node is Full and not Divided, then Subdivide it
        if (!divided) {
            subdivide();
        }

        //inserting Point into Child Nodes
        if (northWest->insert(point)) return true;
        if (northEast->insert(point)) return true;
        if (southWest->insert(point)) return true;
        if (southEast->insert(point)) return true;

        return false;

    }


    //Method for querying a Rectangle. The method will check if the node's boundary and
    //give rectangle intersect. If yes, iterate through all points in the node. If node
    //has children, check them recursively for points. Points stored in the vector will 
    //be printed in the main method.
    void query(Rectangle rectangle, std::vector<Point>& found) {
        //cout << "Querying with rectangle: {" << queryRectangle.x_top_left << ", " << queryRectangle.y_top_left << ", "
            // << queryRectangle.width << ", " << queryRectangle.height << "}" << endl;
        //are rectangles intersecting within the boundary
        if (!rectangle.intersects(boundary)) {
            return;
        }

        // Iterate through all points in the node
        for (const Point& point : points) {
            // Adding the points to the vector if it is inside the query rectangle
            if (rectangle.contains(point)) {
                found.push_back(point);
            }
        }

        // If the node has children, recursively check them for points
        if (divided) {
            northWest->query(rectangle, found);
            northEast->query(rectangle, found);
            southWest->query(rectangle, found);
            southEast->query(rectangle, found);
        }
    }
};

int main() {
    vector<Point> points;
    int numOfPoints, capacity;
    int numOfRectangles;
    float x, y, width, height;

    //Redirected input
    cin >> numOfPoints >> capacity;

    //Read the points and store
    int tempX, tempY;
    for (int i = 0; i < numOfPoints; i++) {
        cin >> tempX >> tempY;
        Point p(tempX, tempY);
        points.push_back(p);
    }

    //Calculate the x, y, width and height of the rectangle for all points.
    //Write code here..
    int minX = points[0].x;
    int minY = points[0].y;
    int maxX = points[0].x;
    int maxY = points[0].y;

    for (int i = 1; i < points.size(); i++) {
        minX = min(minX, points[i].x);
        minY = min(minY, points[i].y);
        maxX = max(maxX, points[i].x);
        maxY = max(maxY, points[i].y);
    }

    width = maxX - minX;//calculating width
    height = maxY - minY;//calculating height

    //Instantiate the tree with the values determined
    Rectangle rectangle(minX, maxY, width, height);
    twoDimSpace twoDSpace(rectangle, capacity);

    //Store the points in the tree.
    //Write code here..

    for (int i = 0;i < points.size();i++) {
        twoDSpace.insert(points[i]);//inserting the points
    }

    cin >> numOfRectangles;
    // Read the query rectangles from the file and query them with the query method in twoDimSpace class.
    for (int i = 0; i < numOfRectangles; i++) {
        cin >> x >> y >> width >> height;
        Rectangle rectangle(x, y, width, height);
        vector<Point> foundPoints;
        twoDSpace.query(rectangle, foundPoints);//calling query method
        sort(foundPoints.begin(), foundPoints.end(), [](const Point& a, const Point& b) {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        });
        cout << endl;
        cout << "Found " << foundPoints.size() << " point(s) in the queried rectangle { "
            << x << ", " << y << ", " << width << ", " << height << " }:" << endl;

        //for (const Point& points : foundPoints) {
          //  cout << "(" << points.x << "," << points.y << ") ";
        //}
int pointsPerLine = 0;

for (const Point& point : foundPoints) {
    if (pointsPerLine == 9) {
        cout << endl; // Start a new line after 9 points
        pointsPerLine = 0; // Reset the counter
    }

    cout << "(" << point.x << "," << point.y << ") ";
    pointsPerLine++;
}
    }
    return 0;

}//End
