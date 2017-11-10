#include <iostream>
#include <memory>
#include <stack>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//receiver - ImageProcessor (ale mogloby tez byc wiecej np, Light, GarageDoor), odbiorca
//invoker - Command Stack, ale powiedzieć ze to moze byc GUI albo coś - przechowuje komendy konkretne
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
    Command(ImageProcessor* proc, Mat& theDst) : proc(proc), dst(theDst) {}
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;

protected:
    ImageProcessor *proc;
    Mat& dst;
};

class MoveRight : public Command
{
public:
    MoveRight(ImageProcessor* proc,  Mat& theDst)
    : Command(proc, theDst) {}

    void execute() override
    {
        dst = proc->translate(dst, 20, 0);
    }

    void undo() override
    {
        dst = proc->translate(dst, -20, 0);
    }
};

class RotateTenDegree : public Command
{
public:
    RotateTenDegree(ImageProcessor* proc, Mat& theDst)
    : Command(proc, theDst) {}

    void execute() override
    {
        dst = proc->rotate(dst, 10);
    }

    void undo() override
    {
        dst = proc->rotate(dst, -10);
    }
};

class CommandStack
{
    using CommandPtr = std::shared_ptr<Command>;
public:
    void execute(CommandPtr comm)
    {
        comm->execute();
        commands.push(comm);
    }

    void undo()
    {
        if(!commands.empty())
        {
            commands.top()->undo();
            commands.pop();
        }
    }
private:
    std::stack<CommandPtr> commands;
};

int main(int argc, char* argv[])
{
    Mat src = imread("logo.jpg");
    int padding = 200;
    Mat canvas(src.rows + padding, src.cols + padding, src.type());
    src.copyTo(canvas(cv::Rect(padding/2, padding/2, src.cols, src.rows)));

    ImageProcessor picasso;
    CommandStack graphicsApp;
    Mat result(canvas);

    std::shared_ptr<Command> moveCmd(new MoveRight(&picasso, result));
    std::shared_ptr<Command> rotateCmd(new RotateTenDegree(&picasso, result));

    imshow("Canvas", canvas);
    imshow("Result", result);
    waitKey(100);

    char ans = 0;
    while(std::cin >> ans)
    {
        switch(ans)
        {
            case 'm':
                graphicsApp.execute(moveCmd);
                break;

            case 'r':
                graphicsApp.execute(rotateCmd);
                break;

            case 'u':
                graphicsApp.undo();
                break;

            case 'q':
                return 0;
            default:
                break;
        }
        imshow("Result", result);
        waitKey(100);
    }

}