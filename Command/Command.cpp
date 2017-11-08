#include <iostream>
#include <memory>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//receiver - ImageProcessor (ale mogloby tez byc wiecej np, Light, GarageDoor)
//invoker - Friendly GUI, ktore umozliwia latwe klikniecie i kilka operacji, w tym undo, makra itp
//client (tworzacy komendy) - main


class ImageProcessor
{
public:
    Mat resize(Mat src, double factor)
    {
        Mat dst;

        cv::resize(src, dst, Size(0, 0), factor, factor);

        return dst;
    }

    Mat translate(Mat src, int x, int y)
    {
        Mat dst;

        Mat tr = Mat::eye( 2, 3, CV_32FC1 );
        // affine_matrix[0][2]:
        *(((float*) (tr.data))+2) = x;
        // affine_matrix [1][2]:
        *(((float*) (tr.data))+5) = y;

        warpAffine(src, dst, tr, Size(src.cols, src.rows));
        return dst;
    }

    Mat rotate(Mat src, double angle)
    {
        Mat dst;
        Point2f pt(src.cols/2., src.rows/2.);
        Mat r = getRotationMatrix2D(pt, angle, 1.0);
        warpAffine(src, dst, r, Size(src.cols, src.rows));
        return dst;
    }
};

class Command
{
public:
    Command(ImageProcessor* theProc, Mat theSrc) : proc(theProc), src(theSrc) {}
    virtual ~Command() = default;
    virtual Mat execute() = 0;
    virtual Mat undo() = 0;

protected:
    ImageProcessor *proc;
    Mat src;
};

class MoveRight : public Command
{
public:
    MoveRight(ImageProcessor* proc, Mat src) : Command(proc, src) {};
    Mat execute()
    {
        src = proc->translate(src, 50, 0);
        return src;
    }

    Mat undo()
    {
        return proc->translate(src, -50, 0);
    }
};

//class UpsideDown50Percent : public Command
//{
//public:
//    UpsideDown50Percent(ImageProcessor* proc, Mat src) : Command(proc, src) {};
//    Mat execute()
//    {
//        Mat dst = proc->resize(src, 0.5);
//
//        return proc->rotate(dst, 180);
//    }
//
//    Mat und
//};

int main(int argc, char* argv[])
{
    Mat src = imread("logo.jpg");
    int padding = 100;
    Mat canvas(src.rows + padding, src.cols + padding, src.type());
    src.copyTo(canvas(cv::Rect(padding/2, padding/2, src.cols, src.rows)));

    ImageProcessor picasso;

    std::shared_ptr<Command> com(new MoveRight(&picasso, canvas));

    auto result = com->execute();

    imshow("Canvas", canvas);
    imshow("Result", result);

    result = com->undo();

    imshow("ResultUndo", result);

    waitKey(0);
}
