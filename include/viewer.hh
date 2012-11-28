/**
 *  @file
 *  @brief 
 *  @date 26/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _OPENGL_HH_
#define _OPENGL_HH_

#include <voronoi/voronoi.hh>

namespace voronoi {

class DiagramViewer {
public:
	DiagramViewer();

	~DiagramViewer();

	static DiagramViewer& getInstance();

	void DisplayCallback();
	void ReshapeCallBack(int width, int height);
	void TimerCallback(int milliseconds);
	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback(int button, int state, int x, int y);
	void MouseMotionCallback(int x, int y);

	void set_diagram(VoronoiDCEL* diagram);
	const VoronoiDCEL* diagram() const;

	void Init();
	void Show();
private:
	void InitDefaultValues();
	void InitGLUT();
	void RegisterCallbacks();
	void InitGL();
	void InitLights();

	void UpdatePerspective(int width, int height);

	bool mouse_x() const;
	bool mouse_y() const;
	bool mouse_left_button() const;
	bool mouse_right_button() const;

	int camera_angle_y() const;
	int camera_angle_x() const;
	float camera_distance() const;

	void set_mouse_x(bool value);
	void set_mouse_y(bool value);
	void set_mouse_left_button(bool value);
	void set_mouse_right_button(bool value);

	void set_camera_angle_y(int value);
	void set_camera_angle_x(int value);
	void set_camera_distance(float value);

	void Draw3DString(const std::string str, const std::vector<float>& pos,
			const std::vector<float>& color, const void* font);

	bool _mouse_x;
	bool _mouse_y;
	bool _mouse_left_button;
	bool _mouse_right_button;

	int _camera_angle_y;
	int _camera_angle_x;
	float _camera_distance;

	VoronoiDCEL* _diagram;

	static const int kWindowPosY;
	static const int kWindowPosX;
	static const int kWindowWidth;
	static const int kWindowHeight;
	static const int kTimerMilliseconds;
	static const float kCameraDistance;
	static const float kCameraAngleY;
	static const float kCameraAngleX;
	static const void* kFont;
};

}

#endif /* _OPENGL_HH_ */
