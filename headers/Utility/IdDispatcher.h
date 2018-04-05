#pragma once

#include <array>

namespace S2D {
	namespace Utility {

		template <size_t MAX_IDS>
		class IdDispatcher
		{
		public:
			IdDispatcher()
			{
				mFirstFreeIndex = 0;
				mNumOfIds = 0;

				for (size_t i = 0; i < mIdArray.size(); i++)
				{
					mIdArray.at(i) = defaultId;
				}
			}

			virtual ~IdDispatcher()
			{
			}

			int32_t createId()
			{
				assert(mNumOfIds + 1 < mIdArray.size());

				// set id in array and search for nearest available index
				int32_t id = mFirstFreeIndex;
				mIdArray.at(mFirstFreeIndex) = id;
				for (size_t i = mFirstFreeIndex + 1; i < mIdArray.size(); i++)
				{
					if (mIdArray.at(i) == defaultId)
					{
						mFirstFreeIndex = i;
						mNumOfIds++;
						break;
					}
				}

				return id;
			}

			void removeId(size_t id)
			{
				assert(id < mIdArray.size());
				if (mIdArray.at(id) != defaultId)
				{
					// remove id
					mIdArray.at(id) = defaultId;
					if (id < mFirstFreeIndex)
					{
						mFirstFreeIndex = id;
					}
					mNumOfIds--;
				}
			}

			bool findId(size_t id)
			{
				return mIdArray.at(id) != defaultId;
			}

			static const unsigned int  defaultId = -1;
		private:

			std::array<int32_t, MAX_IDS> mIdArray;

			size_t mFirstFreeIndex;
			size_t mNumOfIds;
		};
	}
}