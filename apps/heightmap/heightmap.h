// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <QVector2D>
#include <QVector3D>

#include <algorithm>
#include <optional>
#include <vector>

class HeightMap
{
public:
  HeightMap();
  HeightMap(const HeightMap&) = default;
  HeightMap(HeightMap&&) = default;

  const std::vector<float>& zBuffer() const;
  int rows() const;
  int cols() const;

  void fill(std::vector<float> zvalues, int nbcols);

  template<typename F>
  void fill(int nbrows, int nbcols, F&& fun);

  float altMin() const;
  float altMax() const;
  float altSpan() const;
  void setAltitudeRange(float altmin, float altmax);

  std::optional<float> altitudeAt(int row, int col) const;
  float normalizedAltitudeAt(int row, int col) const;

  QVector2D bottomLeft() const;
  QVector2D topRight() const;
  float xRes() const;
  float yRes() const;

  void setGeometry(QVector2D bottom_left, float resolution);
  void setGeometry(QVector2D bottom_left, QVector2D top_right);

  std::optional<QVector3D> pointAt(QVector2D pos) const;

  HeightMap& operator=(const HeightMap&) = default;
  HeightMap& operator=(HeightMap&&) = default;

private:
  std::vector<float> m_z_values;
  int m_cols = 0;
  float m_alt_min = -1;
  float m_alt_max = 1;
  QVector2D m_bottom_left = QVector2D(-1, -1);
  QVector2D m_top_right = QVector2D(1, 1);
};

inline HeightMap::HeightMap()
  : m_z_values(size_t(1), -1.f),
  m_cols(1)
{

}

inline const std::vector<float>& HeightMap::zBuffer() const
{
  return m_z_values;
}

inline int HeightMap::rows() const
{
  return static_cast<int>(zBuffer().size()) / cols();
}

inline int HeightMap::cols() const
{
  return m_cols;
}

inline void HeightMap::fill(std::vector<float> zvalues, int nbcols)
{
  m_z_values = std::move(zvalues);
  m_cols = nbcols;
}

template<typename F>
inline void HeightMap::fill(int nbrows, int nbcols, F&& fun)
{
  m_z_values.resize(nbrows * nbcols);

  size_t i = 0;

  for (int x(0); x < nbcols; ++x)
  {
    for (int y(0); y < nbrows; ++y)
    {
      m_z_values[i++] = fun(x, y);
    }
  }
}

inline float HeightMap::altMin() const
{
  return m_alt_min;
}

inline float HeightMap::altMax() const
{
  return m_alt_max;
}

inline float HeightMap::altSpan() const
{
  return altMax() - altMin();
}

inline void HeightMap::setAltitudeRange(float altmin, float altmax)
{
  m_alt_min = altmin;
  m_alt_max = altmax;
}

inline std::optional<float> HeightMap::altitudeAt(int row, int col) const
{
  float nalt = normalizedAltitudeAt(row, col);
  return nalt < 0.f ? std::optional<float>() : std::optional<float>(altMin() + nalt * altSpan());
}

inline float HeightMap::normalizedAltitudeAt(int row, int col) const
{
  if (row < 0 || col < 0 || row >= rows() || col >= cols())
  {
    return -1.f;
  }
  else
  {
    return m_z_values.at(col * rows() + row);
  }
}

inline QVector2D HeightMap::bottomLeft() const
{
  return m_bottom_left;
}

inline QVector2D HeightMap::topRight() const
{
  return m_top_right;
}

inline float HeightMap::xRes() const
{
  return (topRight().x() - bottomLeft().x()) / cols();
}

inline float HeightMap::yRes() const
{
  return (topRight().y() - bottomLeft().y()) / rows();
}

inline void HeightMap::setGeometry(QVector2D bottom_left, float resolution)
{
  m_bottom_left = bottom_left;
  m_top_right = bottom_left + resolution * QVector2D(cols(), rows());
}

inline void HeightMap::setGeometry(QVector2D bottom_left, QVector2D top_right)
{
  m_bottom_left = bottom_left;
  m_top_right = top_right;
}

inline std::optional<QVector3D> HeightMap::pointAt(QVector2D pos) const
{
  float fcol = (pos.x() - m_bottom_left.x()) / xRes();
  float frow = (pos.y() - m_bottom_left.y()) / yRes();
  int row = static_cast<int>(std::round(frow));
  int col = static_cast<int>(std::round(fcol));

  std::optional<float> z = altitudeAt(std::clamp(row, 0, rows() - 1), std::clamp(col, 0, cols() - 1));

  if (z.has_value()) return QVector3D(pos.x(), pos.y(), z.value());
  else return {};
}

#endif // HEIGHTMAP_H
