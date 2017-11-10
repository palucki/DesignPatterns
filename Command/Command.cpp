#include <iostream>
#include <memory>
#include <stack>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//Example of Command Design Pattern
//actors:
//
//ImageProcessor - receiver of the command,
//                 there can be a lot of them (i.e. multithreaded image processor, logging image processor)
//
//Command Stack  - invoker of the command, stores the commands, it can be GUI, or ThreadPool etc.
//
//main()         - client, creates concrete commands, sets receivers

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
    Command(ImageProcessor* proc, Mat& theDst) : receiver(proc), dst(theDst) {}
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;

protected:
    ImageProcessor *receiver;
    Mat& dst;
};

class MoveRight : public Command
{
public:
    MoveRight(ImageProcessor* proc,  Mat& theDst)
    : Command(proc, theDst) {}

    void execute() override
    {
        dst = receiver->translate(dst, 20, 0);
    }

    void undo() override
    {
        dst = receiver->translate(dst, -20, 0);
    }
};

class RotateTenDegree : public Command
{
public:
    RotateTenDegree(ImageProcessor* proc, Mat& theDst)
    : Command(proc, theDst) {}

    void execute() override
    {
        snapshots.push(dst);
        dst = receiver->rotate(dst, 10);
    }

    void undo() override
    {
        if(!snapshots.empty())
        {
            dst = snapshots.top();
            snapshots.pop();
        }
    }
private:
    std::stack<Mat> snapshots;
};

class CommandStack
{
    using CommandPtr = std::shared_ptr<Command>;
public:
    void execute(CommandPtr cmd)
    {
        cmd->execute();
        commands.push(cmd);
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


void printControls()
{
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Controls: \n";
    std::cout << "m - move image 20 pixels right\n";
    std::cout << "r - rotate image 10 degrees counter clockwise\n";
    std::cout << "u - undo last operation\n";
    std::cout << "q - exit\n";
}

int main(int argc, char* argv[])
{
    Mat src = imread("logo.jpg");
    int padding = 200;
    Mat canvas(src.rows + padding, src.cols + padding, src.type());
    src.copyTo(canvas(cv::Rect(padding/2, padding/2, src.cols, src.rows)));

    ImageProcessor receiver;
    CommandStack invoker;
    Mat result(canvas);

    std::shared_ptr<Command> moveCmd(new MoveRight(&receiver, result));
    std::shared_ptr<Command> rotateCmd(new RotateTenDegree(&receiver, result));

    imshow("Canvas", canvas);
    imshow("Result", result);
    printControls();
    waitKey(100);

    char ans = 0;
    while(std::cin >> ans)
    {
        switch(ans)
        {
            case 'm':
                invoker.execute(moveCmd);
                break;

            case 'r':
                invoker.execute(rotateCmd);
                break;

            case 'u':
                invoker.undo();
                break;

            case 'q':
                return 0;
            default:
                break;
        }
        imshow("Result", result);
        printControls();
        waitKey(100);
    }

}
