#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    if (control_points.size() == 1)
        return control_points[0];

    std::vector<cv::Point2f> points;
    for (int i = 0; i < control_points.size() - 1; i++) {
        auto p = t * control_points[i] + (1 - t) * control_points[i + 1];
        points.push_back(std::move(p));
    }

    return recursive_bezier(points, t);
}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    for (float t = 0.0f; t <= 1.0f; t += 0.001f)
    {
        auto p = recursive_bezier(control_points, t);

        int x = p.x - std::floor(p.x) < 0.5f ? std::floor(p.x) : std::ceil(p.x);
        int y = p.y - std::floor(p.y) < 0.5f ? std::floor(p.y) : std::ceil(p.y);
        std::vector<cv::Point2i> points = {
            cv::Point2i(x, y),
            cv::Point2i(x - 1, y),
            cv::Point2i(x, y - 1),
            cv::Point2i(x - 1, y - 1),
        };

        float sum = 0.0f;
        float max_dis = sqrt(2);
        std::vector<float> distances;
        
        for (const auto& pi: points) {
            float dist = max_dis - std::sqrt(std::pow(pi.x + 0.5f - p.x, 2) + std::pow(pi.y + 0.5f - p.y, 2));
            distances.push_back(dist);
            sum += dist;
        }

        for (int i = 0; i < 4; i++) {
            float k = distances[i] / sum;
            window.at<cv::Vec3b>(points[i].y, points[i].x)[1] = std::min(255.f, window.at<cv::Vec3b>(points[i].y, points[i].x)[1] + 255.f * k);
        }
    }
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4) 
        {
            naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
